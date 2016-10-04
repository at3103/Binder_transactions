# hmwk2-team18
## Problem 1
Set the program to record IPC for the zygote process and start some new applications. Is the zygote doing IPC? If not, what could be the role of zygote?

*Answer:*

Zygote doesn't do IPC, there are no transactions recorded for Zygote; Zygote could be the parent of all applications. In other words, Zygote is responsible for `fork` new applicaiton processes both system and user applications when needed. This can be seen below in the process tree. 

    root      846   1     1504124 72824 poll_sched 719aa53338 S zygote64
    root      847   1     1142284 62148 poll_sched 00f3d3b8d0 S zygote
    audioserver 848   1     22336  5112  binder_thr 00f0991828 S /system/bin/audioserver
    cameraserver 849   1     15792  4316  binder_thr 00e8197828 S /system/bin/cameraserver
    system    981   846   1656912 114440 SyS_epoll_ 719aa53218 S system_server
    ##u0_a39    1173  846   1034308 52116 SyS_epoll_ 719aa53218 S com.android.inputmethod.latin
    media_rw  1190  810   11800  2284  inotify_re 75936e6d10 S /system/bin/sdcard
    ##radio     1365  846   1049476 59700 SyS_epoll_ 719aa53218 S com.android.phone
    ##system    1381  846   1051592 46952 SyS_epoll_ 719aa53218 S com.android.settings
    u0_a29    1472  846   1029856 44940 SyS_epoll_ 719aa53218 S com.android.deskclock
    u0_a10    1551  846   1019032 38836 SyS_epoll_ 719aa53218 S android.ext.services
    u0_a8     1598  846   1025432 47456 SyS_epoll_ 719aa53218 S android.process.media
    system    1611  846   1022164 38996 SyS_epoll_ 719aa53218 S com.android.keychain
    u0_a12    1626  846   1045732 64084 SyS_epoll_ 719aa53218 S com.android.launcher3
    u0_a48    1658  846   1023000 40408 SyS_epoll_ 719aa53218 S com.android.printspooler
    u0_a1     1693  846   1032000 49700 SyS_epoll_ 719aa53218 S android.process.acore
    root      1721  2     0      0     worker_thr 0000000000 S kworker/u2:2
    u0_a19    1741  846   1092672 98864 SyS_epoll_ 719aa53218 S com.android.systemui
    u0_a50    1775  846   1023440 40864 SyS_epoll_ 719aa53218 S com.android.quicksearchbox
    root      1813  1319  9096   1616           0 727de73d10 R ps

## Problem 2
Set the program to record IPC for a number of applications and use the applications while you're recording IPC. Is there some set of common processes that all applications seem to communicate with? Why could that be?

*Answer:*
```
    generic_arm64:/ # /data/misc/binder_info print 2446                                          
    droid.deskclock (2446):	23036 bytes	162 transactions
        surfaceflinger	840	1003
        system_server	984	1000
        putmethod.latin	1172	10039
        servicemanager	839	1000

    generic_arm64:/ # /data/misc/binder_info print 2394                                          
    oid.calculator2 (2394):	19248 bytes	172 transactions
        surfaceflinger	840	1003
        system_server	984	1000
        servicemanager	839	1000
        putmethod.latin	1172	10039

    .android.camera (2198):	28504 bytes	210 transactions
        system_server	984	1000
        servicemanager	839	1000
        cameraserver	848	1005
        surfaceflinger	840	1003

    generic_arm64:/ # /data/misc/binder_info print 2089                                          
    .android.dialer (2089):	44684 bytes	363 transactions
        system_server	984	1000
        surfaceflinger	840	1003
        servicemanager	839	1000
        audioserver	847	1005
        d.process.acore	1702	10001
        putmethod.latin	1172	10039

    droid.gallery3d (1936):	15832 bytes	125 transactions
        system_server	984	1000
        servicemanager	839	1000
        surfaceflinger	840	1003
        d.process.media	1620	10008
        putmethod.latin	1172	10039

    generic_arm64:/ # /data/misc/binder_info print 1848                                          
    droid.messaging (1848):	143100 bytes	1022 transactions
        system_server	984	1000
        servicemanager	839	1000
        m.android.phone	1366	1001
        surfaceflinger	840	1003
        putmethod.latin	1172	10039
        d.process.acore	1702	10001

    generic_arm64:/ # /data/misc/binder_info print 1796                                          
    m.android.email (1796):	23716 bytes	153 transactions
        system_server	984	1000
        servicemanager	839	1000
        surfaceflinger	840	1003
        putmethod.latin	1172	10039
        d.process.acore	1702	10001
```

Yes. All the applciations listed above communicate with system_server, servicemanager and surfaceflinger. And almost all of the applications communicate with putmethod.latin with the exception of .android.camera. Almost all the applications communicate with a common set of processes, because these processes are very significant and critical to the smooth and efficient funcitoning of these applicaitons. These critical processes seem to form a common set among various applicaitons. 

All the applications communicate with surfaceflinger becuase it is the android process that controls the screen. In most of the present android phones and in particular the adroid model which we are using as an emualtor have the screen as the primary mode of input. Hence, in order to use any of the feature of these applications, the input has to be received from the user via the surfaceflinger which controls the screen. Hence, all the applications communicate with surface flinger.

Similarly, all the processes communicate with the servicemanager which allocates resources and manages the different services needed for the functioning of the android applications. 

system_server is the one of the most critical process of the Android OS. Hence, all the applications communicate with this process.

## Problem 3
Calculate the average size of a Binder message. What does the size of the Binder messages tell you about the type of IPC messages being passed through Binder. Which other IPC mechanism may be a better choice for sharing very large amounts of data between applications?

*Answer:*

Around 100 bytes; The Binder messages are not very big, meaning there are not significant data exchanges between processes that passes through Binder, most probably signals; Files are better choices for IPC in extremely large scale, as it uses hard disk to store large amount data.

