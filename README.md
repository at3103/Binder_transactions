# hmwk2-team18


Reason about Android IPC



Set the program to record IPC for a number of applications and 
use the applications while you're recording IPC.
Is there some set of common processes that all applications 
seem to communicate with? Why could that be?

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



  Yes. All the applciations listed above communicate with system_server, servicemanager 
  and surfaceflinger. And almost all of the applications communicate with putmethod.latin 
  with the exception of .android.camera. Almost all the applications communicate with 
  a common set of processes, because these processes are very significant and critical
  to the smooth and efficient funcitoning of these applicaitons. These critical processes
  seem to form a common set among various applicaitons. 
  
  All the applications communicate with surfaceflinger becuase it is the android process
  that controls the screen. In most of the present android phones and in particular the 
  adroid model which we are using as an emualtor have the screen as the primary mode of 
  input. Hence, in order to use any of the feature of these applications, the input has
  to be received from the user via the surfaceflinger which controls the screen. Hence,
  all the applications communicate with surface flinger.
  
  Similarly, all the processes communicate with the servicemanager which allocates
  resources and manages the different services needed for the functioning of the 
  android applications. 
  
  system_server is the one of the most critical process of the Android OS. Hence,
  all the applications communicate with this process.

