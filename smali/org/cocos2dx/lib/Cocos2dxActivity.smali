.class public Lorg/cocos2dx/lib/Cocos2dxActivity;
.super Lcom/customRobTop/BaseRobTopActivity;


# static fields
.field private static final HANDLER_SHOW_DIALOG:I = 0x1

.field private static final PICK_MP3_REQUEST:I = 0x1

.field public static _app:Lorg/cocos2dx/lib/Cocos2dxActivity;

.field private static accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

.field private static accelerometerEnabled:Z

.field private static backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

.field private static handler:Landroid/os/Handler;

.field private static me:Lorg/cocos2dx/lib/Cocos2dxActivity;

.field private static packageName:Ljava/lang/String;

.field private static soundPlayer:Lorg/cocos2dx/lib/q;


# direct methods
.method static constructor <clinit>()V
    .locals 1

    const/4 v0, 0x0

    sput-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    const/4 v0, 0x0

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->me:Lorg/cocos2dx/lib/Cocos2dxActivity;

    return-void
.end method

.method public constructor <init>()V
    .locals 0

    invoke-direct {p0}, Lcom/customRobTop/BaseRobTopActivity;-><init>()V

    return-void
.end method

.method static synthetic access$0(Lorg/cocos2dx/lib/Cocos2dxActivity;Ljava/lang/String;Ljava/lang/String;)V
    .locals 0

    invoke-direct {p0, p1, p2}, Lorg/cocos2dx/lib/Cocos2dxActivity;->showDialog(Ljava/lang/String;Ljava/lang/String;)V

    return-void
.end method

.method public static disableAccelerometer()V
    .locals 1

    const/4 v0, 0x0

    sput-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->disable()V

    return-void
.end method

.method public static enableAccelerometer()V
    .locals 1

    const/4 v0, 0x1

    sput-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->enable()V

    return-void
.end method

.method public static end()V
    .locals 1

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/p;->end()V

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/q;->end()V

    return-void
.end method

.method public static getBackgroundMusicVolume()F
    .locals 1

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/p;->getBackgroundVolume()F

    move-result v0

    return v0
.end method

.method public static getCocos2dxPackageName()Ljava/lang/String;
    .locals 1

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->packageName:Ljava/lang/String;

    return-object v0
.end method

.method public static getCurrentLanguage()Ljava/lang/String;
    .locals 1

    invoke-static {}, Ljava/util/Locale;->getDefault()Ljava/util/Locale;

    move-result-object v0

    invoke-virtual {v0}, Ljava/util/Locale;->getLanguage()Ljava/lang/String;

    move-result-object v0

    return-object v0
.end method

.method public static getEffectsVolume()F
    .locals 1

    const/4 v0, 0x0

    return v0

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/q;->getEffectsVolume()F

    move-result v0

    return v0
.end method

.method public static isBackgroundMusicPlaying()Z
    .locals 1

    const/4 v0, 0x0

    return v0

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/p;->isBackgroundMusicPlaying()Z

    move-result v0

    return v0
.end method

.method private static native nativeSetPaths(Ljava/lang/String;)V
.end method

.method public static openMp3Selector()V
    .locals 3

    new-instance v0, Landroid/content/Intent;

    const-string v1, "android.intent.action.GET_CONTENT"

    invoke-direct {v0, v1}, Landroid/content/Intent;-><init>(Ljava/lang/String;)V

    const-string v1, "audio/mpeg"

    invoke-virtual {v0, v1}, Landroid/content/Intent;->setType(Ljava/lang/String;)Landroid/content/Intent;

    const-string v1, "Select MP3"

    invoke-static {v0, v1}, Landroid/content/Intent;->createChooser(Landroid/content/Intent;Ljava/lang/CharSequence;)Landroid/content/Intent;

    move-result-object v0

    sget-object v1, Lorg/cocos2dx/lib/Cocos2dxActivity;->_app:Lorg/cocos2dx/lib/Cocos2dxActivity;

    const/4 v2, 0x1

    invoke-virtual {v1, v0, v2}, Lorg/cocos2dx/lib/Cocos2dxActivity;->startActivityForResult(Landroid/content/Intent;I)V

    return-void
.end method

.method public static pauseAllEffects()V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/q;->pauseAllEffects()V

    return-void
.end method

.method public static pauseBackgroundMusic()V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/p;->pauseBackgroundMusic()V

    return-void
.end method

.method public static pauseEffect(I)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/q;->pauseEffect(I)V

    return-void
.end method

.method public static playBackgroundMusic(Ljava/lang/String;Z)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0, p0, p1}, Lorg/cocos2dx/lib/p;->playBackgroundMusic(Ljava/lang/String;Z)V

    return-void
.end method

.method public static playEffect(Ljava/lang/String;ZFFF)I
    .locals 6

    const/4 v0, 0x0

    return v0

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    move-object v1, p0

    move v2, p1

    move v3, p2

    move v4, p3

    move v5, p4

    invoke-virtual/range {v0 .. v5}, Lorg/cocos2dx/lib/q;->playEffect(Ljava/lang/String;ZFFF)I

    move-result v0

    return v0
.end method

.method public static preloadBackgroundMusic(Ljava/lang/String;)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/p;->preloadBackgroundMusic(Ljava/lang/String;)V

    return-void
.end method

.method public static preloadEffect(Ljava/lang/String;)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/q;->preloadEffect(Ljava/lang/String;)I

    return-void
.end method

.method public static resumeAllEffects()V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/q;->resumeAllEffects()V

    return-void
.end method

.method public static resumeBackgroundMusic()V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/p;->resumeBackgroundMusic()V

    return-void
.end method

.method public static resumeEffect(I)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/q;->resumeEffect(I)V

    return-void
.end method

.method public static rewindBackgroundMusic()V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/p;->rewindBackgroundMusic()V

    return-void
.end method

.method public static setBackgroundMusicTime(F)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/p;->setBackgroundMusicTime(F)V

    return-void
.end method

.method public static setBackgroundMusicVolume(F)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/p;->setBackgroundVolume(F)V

    return-void
.end method

.method public static setEffectsVolume(F)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/q;->setEffectsVolume(F)V

    return-void
.end method

.method private showDialog(Ljava/lang/String;Ljava/lang/String;)V
    .locals 3

    new-instance v0, Landroid/app/AlertDialog$Builder;

    invoke-direct {v0, p0}, Landroid/app/AlertDialog$Builder;-><init>(Landroid/content/Context;)V

    invoke-virtual {v0, p1}, Landroid/app/AlertDialog$Builder;->setTitle(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v0

    invoke-virtual {v0, p2}, Landroid/app/AlertDialog$Builder;->setMessage(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v0

    const-string v1, "Ok"

    new-instance v2, Lorg/cocos2dx/lib/b;

    invoke-direct {v2, p0}, Lorg/cocos2dx/lib/b;-><init>(Lorg/cocos2dx/lib/Cocos2dxActivity;)V

    invoke-virtual {v0, v1, v2}, Landroid/app/AlertDialog$Builder;->setPositiveButton(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;

    move-result-object v0

    invoke-virtual {v0}, Landroid/app/AlertDialog$Builder;->create()Landroid/app/AlertDialog;

    move-result-object v0

    invoke-virtual {v0}, Landroid/app/Dialog;->show()V

    return-void
.end method

.method public static showMessageBox(Ljava/lang/String;Ljava/lang/String;)V
    .locals 2

    new-instance v0, Landroid/os/Message;

    invoke-direct {v0}, Landroid/os/Message;-><init>()V

    const/4 v1, 0x1

    iput v1, v0, Landroid/os/Message;->what:I

    new-instance v1, Lorg/cocos2dx/lib/s;

    invoke-direct {v1, p0, p1}, Lorg/cocos2dx/lib/s;-><init>(Ljava/lang/String;Ljava/lang/String;)V

    iput-object v1, v0, Landroid/os/Message;->obj:Ljava/lang/Object;

    sget-object v1, Lorg/cocos2dx/lib/Cocos2dxActivity;->handler:Landroid/os/Handler;

    invoke-virtual {v1, v0}, Landroid/os/Handler;->sendMessage(Landroid/os/Message;)Z

    return-void
.end method

.method public static stopAllEffects()V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/q;->stopAllEffects()V

    return-void
.end method

.method public static stopBackgroundMusic()V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/p;->stopBackgroundMusic()V

    return-void
.end method

.method public static stopEffect(I)V
    .locals 1

    return-void

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/q;->stopEffect(I)V

    return-void
.end method

.method public static terminateProcess()V
    .locals 1

    invoke-static {}, Landroid/os/Process;->myPid()I

    move-result v0

    invoke-static {v0}, Landroid/os/Process;->killProcess(I)V

    return-void
.end method

.method public static unloadEffect(Ljava/lang/String;)V
    .locals 1

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/q;->unloadEffect(Ljava/lang/String;)V

    return-void
.end method


# virtual methods
.method public native nativeOnMp3Selected([BJJ)V
.end method

.method protected onActivityResult(IILandroid/content/Intent;)V
    .locals 10

    const/4 v0, 0x1
    if-ne p1, v0, :cond_exit
    const/4 v0, -0x1
    if-ne p2, v0, :cond_exit
    if-nez p3, :cond_exit

    invoke-virtual {p3}, Landroid/content/Intent;->getData()Landroid/net/Uri;
    move-result-object v0
    if-nez v0, :cond_exit

    # 1. Get Duration
    new-instance v1, Landroid/media/MediaMetadataRetriever;
    invoke-direct {v1}, Landroid/media/MediaMetadataRetriever;-><init>()V
    :try_start_0
    invoke-virtual {v1, p0, v0}, Landroid/media/MediaMetadataRetriever;->setDataSource(Landroid/content/Context;Landroid/net/Uri;)V
    const/16 v2, 0x9
    invoke-virtual {v1, v2}, Landroid/media/MediaMetadataRetriever;->extractMetadata(I)Ljava/lang/String;
    move-result-object v2
    invoke-static {v2}, Ljava/lang/Long;->parseLong(Ljava/lang/String;)J
    move-result-wide v2 # Results in v2, v3
    invoke-virtual {v1}, Landroid/media/MediaMetadataRetriever;->release()V
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_metadata

    :goto_0
    # 2. Get Data
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->getContentResolver()Landroid/content/ContentResolver;
    move-result-object v1
    invoke-virtual {v1, v0}, Landroid/content/ContentResolver;->openInputStream(Landroid/net/Uri;)Ljava/io/InputStream;
    move-result-object v0
    invoke-static {v0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->readAllBytes(Ljava/io/InputStream;)[B
    move-result-object v1 # v1 = byte[]

    # 3. Get Size
    array-length v4, v1
    int-to-long v4, v4 # v4, v5 = size

    # 4. Correct Invoke
    # Range must be {v0, v1, v2, v3, v4, v5} (6 slots)
    move-object v0, p0 # v0 = this
    # v1 = [B, v2/v3 = duration, v4/v5 = size
    invoke-virtual/range {v0 .. v5}, Lorg/cocos2dx/lib/Cocos2dxActivity;->nativeOnMp3Selected([BJJ)V

    :cond_exit
    invoke-super {p0, p1, p2, p3}, Landroid/app/Activity;->onActivityResult(IILandroid/content/Intent;)V
    return-void

    :catch_metadata
    const-wide/16 v2, 0x0
    goto :goto_0
.end method



.method public static readAllBytes(Ljava/io/InputStream;)[B
    .locals 4
    new-instance v0, Ljava/io/ByteArrayOutputStream;
    invoke-direct {v0}, Ljava/io/ByteArrayOutputStream;-><init>()V
    const/16 v1, 0x400
    new-array v1, v1, [B
    :loop
    invoke-virtual {p0, v1}, Ljava/io/InputStream;->read([B)I
    move-result v2
    const/4 v3, -0x1
    if-eq v2, v3, :cond_0
    const/4 v3, 0x0
    invoke-virtual {v0, v1, v3, v2}, Ljava/io/ByteArrayOutputStream;->write([BII)V
    goto :loop
    :cond_0
    invoke-virtual {v0}, Ljava/io/ByteArrayOutputStream;->toByteArray()[B
    move-result-object v0
    return-object v0
.end method


.method protected onCreate(Landroid/os/Bundle;)V
    .locals 2

    sput-object p0, Lorg/cocos2dx/lib/Cocos2dxActivity;->_app:Lorg/cocos2dx/lib/Cocos2dxActivity;

    invoke-super {p0, p1}, Lcom/customRobTop/BaseRobTopActivity;->onCreate(Landroid/os/Bundle;)V

    invoke-static {p0}, Lorg/fmod/FMOD;->init(Landroid/content/Context;)V

    new-instance v0, Landroid/util/DisplayMetrics;

    invoke-direct {v0}, Landroid/util/DisplayMetrics;-><init>()V

    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->getWindowManager()Landroid/view/WindowManager;

    move-result-object v1

    invoke-interface {v1}, Landroid/view/WindowManager;->getDefaultDisplay()Landroid/view/Display;

    move-result-object v1

    invoke-virtual {v1, v0}, Landroid/view/Display;->getMetrics(Landroid/util/DisplayMetrics;)V

    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;-><init>(Landroid/content/Context;)V

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    if-eqz v0, :cond_0

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    iput-object p0, v0, Lorg/cocos2dx/lib/p;->mContext:Landroid/content/Context;

    :goto_0
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    if-eqz v0, :cond_1

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    iput-object p0, v0, Lorg/cocos2dx/lib/q;->mContext:Landroid/content/Context;

    :goto_1
    invoke-static {p0}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->setContext(Landroid/content/Context;)V

    new-instance v0, Lorg/cocos2dx/lib/a;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/a;-><init>(Lorg/cocos2dx/lib/Cocos2dxActivity;)V

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->handler:Landroid/os/Handler;

    return-void

    :cond_0
    new-instance v0, Lorg/cocos2dx/lib/p;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/p;-><init>(Landroid/content/Context;)V

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/p;

    goto :goto_0

    :cond_1
    new-instance v0, Lorg/cocos2dx/lib/q;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/q;-><init>(Landroid/content/Context;)V

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/q;

    goto :goto_1
.end method

.method protected onPause()V
    .locals 1

    invoke-super {p0}, Lcom/customRobTop/BaseRobTopActivity;->onPause()V

    sget-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    if-eqz v0, :cond_0

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->disable()V

    :cond_0
    return-void
.end method

.method protected onResume()V
    .locals 1

    invoke-super {p0}, Lcom/customRobTop/BaseRobTopActivity;->onResume()V

    sget-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    if-eqz v0, :cond_0

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->enable()V

    :cond_0
    return-void
.end method

.method protected setPackageName(Ljava/lang/String;)V
    .locals 2

    sput-object p1, Lorg/cocos2dx/lib/Cocos2dxActivity;->packageName:Ljava/lang/String;

    const-string v0, ""

    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->getApplication()Landroid/app/Application;

    move-result-object v0

    invoke-virtual {v0}, Landroid/app/Application;->getPackageManager()Landroid/content/pm/PackageManager;

    move-result-object v0

    const/4 v1, 0x0

    :try_start_0
    invoke-virtual {v0, p1, v1}, Landroid/content/pm/PackageManager;->getApplicationInfo(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;
    :try_end_0
    .catch Landroid/content/pm/PackageManager$NameNotFoundException; {:try_start_0 .. :try_end_0} :catch_0

    move-result-object v0

    iget-object v0, v0, Landroid/content/pm/ApplicationInfo;->sourceDir:Ljava/lang/String;

    const-string v1, "apk path"

    invoke-static {v1, v0}, Landroid/util/Log;->w(Ljava/lang/String;Ljava/lang/String;)I

    invoke-static {v0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->nativeSetPaths(Ljava/lang/String;)V

    return-void

    :catch_0
    move-exception v0

    invoke-virtual {v0}, Landroid/content/pm/PackageManager$NameNotFoundException;->printStackTrace()V

    new-instance v0, Ljava/lang/RuntimeException;

    const-string v1, "Unable to locate assets, aborting..."

    invoke-direct {v0, v1}, Ljava/lang/RuntimeException;-><init>(Ljava/lang/String;)V

    throw v0
.end method
