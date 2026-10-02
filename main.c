#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/vmalloc.h>

#define MY_NAME     "myblk"
#define NSECTORS    32768
#define SECTOR_SIZE 512

static struct blk_mq_tag_set tag_set;
static struct request_queue *queue;
static struct gendisk *disk;
static u8 *storage;

static const struct block_device_operations myblk_fops = {
    .owner = THIS_MODULE,
};

static blk_status_t myblk_queue_rq(
    struct blk_mq_hw_ctx *hctx,
    const struct blk_mq_queue_data *bd)
{
    struct request *rq = bd->rq;
    struct bio_vec bvec;
    struct req_iterator iter;
    sector_t sector = blk_rq_pos(rq);

    blk_mq_start_request(rq);

    pr_info("myblk: request received\n");
    pr_info("myblk: operation = %u\n", req_op(rq));
    pr_info("myblk: sector = %llu\n",
            (unsigned long long)sector);
    pr_info("myblk: bytes = %u\n", blk_rq_bytes(rq));

    if (sector + (blk_rq_bytes(rq) >> 9) > NSECTORS) {
        pr_err("myblk: request exceeds device capacity\n");
        blk_mq_end_request(rq, BLK_STS_IOERR);
        return BLK_STS_IOERR;
    }

    if (req_op(rq) == REQ_OP_READ) {

        rq_for_each_segment(bvec, rq, iter) {
            unsigned int len = bvec.bv_len;
            void *buffer;

            buffer = kmap_local_page(bvec.bv_page);

            memcpy(
                buffer + bvec.bv_offset,
                storage + (sector << 9),
                len
            );

            kunmap_local(buffer);

            sector += len >> 9;
        }

    } else if (req_op(rq) == REQ_OP_WRITE) {

        rq_for_each_segment(bvec, rq, iter) {
            unsigned int len = bvec.bv_len;
            void *buffer;

            buffer = kmap_local_page(bvec.bv_page);

            memcpy(
                storage + (sector << 9),
                buffer + bvec.bv_offset,
                len
            );

            kunmap_local(buffer);

            sector += len >> 9;
        }

    } else {
        pr_info("myblk: unsupported operation = %u\n",
                req_op(rq));
    }

    blk_mq_end_request(rq, BLK_STS_OK);

    return BLK_STS_OK;
}

static const struct blk_mq_ops myblk_mq_ops = {
    .queue_rq = myblk_queue_rq,
};

static int __init myblk_init(void)
{
    int ret;

    pr_info("myblk: initializing\n");

    storage = vmalloc(NSECTORS * SECTOR_SIZE);

    if (!storage) {
        pr_err("myblk: failed to allocate storage\n");
        return -ENOMEM;
    }

    memset(storage, 0, NSECTORS * SECTOR_SIZE);

    pr_info("myblk: allocated %u bytes of storage\n",
            NSECTORS * SECTOR_SIZE);

    tag_set.ops = &myblk_mq_ops;
    tag_set.nr_hw_queues = 1;
    tag_set.queue_depth = 16;

    ret = blk_mq_alloc_tag_set(&tag_set);

    if (ret) {
        pr_err("myblk: failed to allocate tag set\n");
        vfree(storage);
        storage = NULL;
        return ret;
    }

    pr_info("myblk: tag set allocated\n");

    queue = blk_mq_alloc_queue(
        &tag_set,
        NULL,
        NULL
    );

    if (IS_ERR(queue)) {
        ret = PTR_ERR(queue);

        pr_err("myblk: failed to allocate request queue\n");

        blk_mq_free_tag_set(&tag_set);
        vfree(storage);
        storage = NULL;

        return ret;
    }

    pr_info("myblk: request queue allocated\n");

    disk = blk_mq_alloc_disk_for_queue(
        queue,
        NULL
    );

    if (IS_ERR(disk)) {
        ret = PTR_ERR(disk);

        pr_err("myblk: failed to allocate gendisk\n");

        blk_mq_destroy_queue(queue);
        blk_mq_free_tag_set(&tag_set);
        vfree(storage);
        storage = NULL;

        return ret;
    }

    pr_info("myblk: gendisk allocated\n");

    disk->fops = &myblk_fops;

    strscpy(
        disk->disk_name,
        MY_NAME,
        DISK_NAME_LEN
    );

    set_capacity(
        disk,
        NSECTORS
    );

    ret = device_add_disk(
        NULL,
        disk,
        NULL
    );

    if (ret) {
        pr_err("myblk: failed to add disk\n");

        put_disk(disk);
        blk_mq_destroy_queue(queue);
        blk_mq_free_tag_set(&tag_set);
        vfree(storage);
        storage = NULL;

        return ret;
    }

    pr_info("myblk: disk registered successfully\n");
    pr_info("myblk: module loaded\n");

    return 0;
}

static void __exit myblk_exit(void)
{
    pr_info("myblk: removing module\n");

    if (disk) {
        del_gendisk(disk);
        put_disk(disk);
        disk = NULL;
    }

    if (queue) {
        blk_mq_destroy_queue(queue);
        queue = NULL;
    }

    blk_mq_free_tag_set(&tag_set);

    if (storage) {
        vfree(storage);
        storage = NULL;
    }

    pr_info("myblk: module unloaded\n");
}

module_init(myblk_init);
module_exit(myblk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vishnu");
MODULE_DESCRIPTION("Learning Linux blk-mq RAM-backed block driver");