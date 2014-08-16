/*
  վերնագրային ֆայլեր, որոնք հարկավոր են
  մոդուլի ճիշտ աշխատանքի համար
*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>


#define RETVAL_SUCC 0
#define DEVICE_NAME "zch"

/*
  Մակրոսներ, մոդուլի մասին
  ինֆորմացիա տրամադրելու համար

*/

// լիցենզիայի տիպը
MODULE_LICENSE("GPL");

// հեղինակը
MODULE_AUTHOR("Davit Petrosyan <da... at gmail>");

// նկարագրություն
MODULE_DESCRIPTION("char-based /dev/zero alternative");

// աջակցվող սարքի անվանում
// մեր դեպքում՝ /dev/zch
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);


// սարքի վերին համարը (major number)
// սարքերի վերին և ստորին (major and minor)
// համարները տեսնելու համար
// անցեք /dev պանակ և կատարեք ls -al
// տարեթվից առաջ, ստորակետով բաժանված
// թվերը համապատասխանաբար major և minor
// թվերն են
static int major;

// զրո սիմվոլը
// հենց այս սիմվոլն է սարքը վերադարձնում որպես ելք
static char zch = '0';

// ինտերֆեյս, որը տրամադրելու է վիրտուալ սարքը
// ավելի ֆունկցիոնալ ինտերֆեյս տրամադրելու համար
// ծանոթացեք file_operation կառուցվածքի հետ
// հղումներ՝ http://www.tldp.org/LDP/lkmpg/2.4/html/c577.htm
//           http://www.makelinux.net/ldd3/chp-3-sect-3

// սարքի մեկնարկ
static int zch_open(struct inode*, struct file*);
// սարքից ընթերցում
static ssize_t zch_read(struct file*, char*, size_t, loff_t*);
// սարքի հեռացում
static int zch_release(struct inode*, struct file*);


// գրանցում ենք հայտարարված ինտերֆեյսը
// համապատասխան կառուցվածքում (file_operations)
static struct file_operations zch_fop = {
        .read = zch_read,
        .open = zch_open,
        .release = zch_release
    };


// մոդուլի սկզբնարժեքավորում ֆունկցիա
static int __init zch_module_init(void){
    // printk-ն printf-ի անալոգն է Kernel֊ում
    printk(KERN_ALERT "ZeroChar (zch) device is ready \n");

    // գրանցում ենք սարքը և ստանում ենք սարքի համար
    // վերին համար
    major = register_chrdev(0, DEVICE_NAME, &zch_fop);

    // եթե չհաջողվեց (major-ը բացասական թիվ է)
    if(major < 0){
        printk("Failed to register char device with error number: %d\n", major);
        return major;
    }


    // մոդուլը ակտիվ է
    printk("ZCH device module is ready\n");

    // սարքի ստեղծման հիշեցում
    printk("Create device with 'mknod /dev/zch c %d 0'.\n", major);

    return RETVAL_SUCC;
}


// սարքի մեկնարկ
static int zch_open( struct inode *inode, struct file *file ){
    // ինչ֊որ նախապատրաստական
    // աշխատանքներ
    // օրինակ սարքի զուգահեռ
    // մեկնարկի սինխրոնիզացիա

    return RETVAL_SUCC;
}

// սարքի հեռացում
static int zch_release(struct inode* inode, struct file* file){
    // ռեսուրսների ազատում
    // օրինակ, եթե բուֆերային
    // սարքերի դեպքում
    // պետք է բուֆերը դատարկել

    return RETVAL_SUCC;
}


// կարդալու գործողության նկարագրություն
static ssize_t zch_read( struct file *filp,
       char* buffer,
       size_t length,
       loff_t* offset){

    // քանի բայթ է ստացվել
    int bytes_read = 0;


    while(length){
        // put_user մակրոն
        // օգտագործվում է Kernel Space-ում
        // գտնվող ինֆորմացիան user space
        // տեղափոխելու համար
        put_user(zch, buffer);

        // բուֆերի ցուցիչի տեղաշարժ
        buffer++;

        // երկարությունը մեկով պակասեց
        // քանի որ մեկ բայթ մուտքագրվեց
        length--;

        // ստացված բայթերի քանակը մեկով ավելացավ
        bytes_read++;
    }

    return bytes_read;
}


// մոդուլի հեռացում
static void __exit zch_module_exit(void){
    // սարքի հեռացում
    unregister_chrdev(major, DEVICE_NAME);

    printk(KERN_ALERT "ZeroChar device module is unloaded \n");
}


// մոդուլի "կոնստրուկտորի" ու
// "դեստրուկտորի" գրանցում
module_init(zch_module_init);
module_exit(zch_module_exit);
