.class public Lcom/robtopx/geometryjump/GeometryJump;
.super Lorg/cocos2dx/lib/Cocos2dxActivity;


# static fields
.field public static final SCREEN_ORIENTATION_LANDSCAPE:I = 0x0

.field public static final SCREEN_ORIENTATION_SENSOR_LANDSCAPE:I = 0x6

.field public static final SCREEN_ORIENTATION_USER_LANDSCAPE:I = 0xb


# instance fields
.field private mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;


# direct methods
.method static constructor <clinit>()V
    .locals 2

    const-string v0, "fmodL"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    const-string v0, "game"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    const-string v0, "violet"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    return-void
.end method

.method public constructor <init>()V
    .locals 0

    invoke-direct {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;-><init>()V

    return-void
.end method

.method private detectOpenGLES20()Z
    .locals 2

    const-string v0, "activity"

    invoke-virtual {p0, v0}, Lcom/robtopx/geometryjump/GeometryJump;->getSystemService(Ljava/lang/String;)Ljava/lang/Object;

    move-result-object v0

    check-cast v0, Landroid/app/ActivityManager;

    invoke-virtual {v0}, Landroid/app/ActivityManager;->getDeviceConfigurationInfo()Landroid/content/pm/ConfigurationInfo;

    move-result-object v0

    iget v0, v0, Landroid/content/pm/ConfigurationInfo;->reqGlEsVersion:I

    const/high16 v1, 0x20000

    if-ge v0, v1, :cond_0

    sget-object v0, Landroid/os/Build;->FINGERPRINT:Ljava/lang/String;

    const-string v1, "generic"

    invoke-virtual {v0, v1}, Ljava/lang/String;->startsWith(Ljava/lang/String;)Z

    move-result v0

    if-nez v0, :cond_0

    const/4 v0, 0x0

    :goto_0
    return v0

    :cond_0
    const/4 v0, 0x1

    goto :goto_0
.end method


# virtual methods
.method public getPublicKey()Ljava/lang/String;
    .locals 1

    const-string v0, "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgRyoIFy3izJ9+RBfum3Fvc6iiuk6TFst1yaPUUaThzD/6wlHwCDjVjnoDaqL29HE3j2VQpv5rmdSvDZXayxa1861//Ca3p3aDvlvcxZwyN6Fe51patKofmVNp9DhofDcsMNLLAfJMqihwgRCBHToi2B3xrstd+233UCgzz4oMg1cClfuSHn1YF7clksIrjuTrlBq1fmn3Km4E7S90ghyzL0KxT7gr1dyQdu8IuK++psnvv4wKybsKyuGm6fLk/X20TRJObi2Y8CRv4Zf9gWxSndR8sevQ1tOhQp1VQWWZqhks9r4DwPclEl2Ox0kREs4wR9zFdaWodFb1eKG3oNmPwIDAQAB"

    return-object v0
.end method

.method protected onCreate(Landroid/os/Bundle;)V
    .locals 5

    const v4, 0x80001

    const/4 v3, -0x1

    invoke-super {p0, p1}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onCreate(Landroid/os/Bundle;)V

    sget v0, Landroid/os/Build$VERSION;->SDK_INT:I

    const/16 v1, 0x12

    if-lt v0, v1, :cond_0

    const/16 v0, 0xb

    invoke-virtual {p0, v0}, Lcom/robtopx/geometryjump/GeometryJump;->setRequestedOrientation(I)V

    :goto_0
    invoke-direct {p0}, Lcom/robtopx/geometryjump/GeometryJump;->detectOpenGLES20()Z

    move-result v0

    if-eqz v0, :cond_2

    invoke-virtual {p0}, Lcom/robtopx/geometryjump/GeometryJump;->getApplication()Landroid/app/Application;

    move-result-object v0

    invoke-virtual {v0}, Landroid/app/Application;->getPackageName()Ljava/lang/String;

    move-result-object v0

    invoke-super {p0, v0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->setPackageName(Ljava/lang/String;)V

    new-instance v0, Landroid/view/ViewGroup$LayoutParams;

    invoke-direct {v0, v3, v3}, Landroid/view/ViewGroup$LayoutParams;-><init>(II)V

    new-instance v1, Landroid/widget/FrameLayout;

    invoke-direct {v1, p0}, Landroid/widget/FrameLayout;-><init>(Landroid/content/Context;)V

    invoke-virtual {v1, v0}, Landroid/widget/FrameLayout;->setLayoutParams(Landroid/view/ViewGroup$LayoutParams;)V

    new-instance v0, Landroid/view/ViewGroup$LayoutParams;

    const/4 v2, -0x2

    invoke-direct {v0, v3, v2}, Landroid/view/ViewGroup$LayoutParams;-><init>(II)V

    new-instance v2, Lorg/cocos2dx/lib/Cocos2dxEditText;

    invoke-direct {v2, p0}, Lorg/cocos2dx/lib/Cocos2dxEditText;-><init>(Landroid/content/Context;)V

    invoke-virtual {v2, v0}, Lorg/cocos2dx/lib/Cocos2dxEditText;->setLayoutParams(Landroid/view/ViewGroup$LayoutParams;)V

    invoke-virtual {v2, v4}, Lorg/cocos2dx/lib/Cocos2dxEditText;->setInputType(I)V

    invoke-virtual {v1, v2}, Landroid/widget/FrameLayout;->addView(Landroid/view/View;)V

    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;-><init>(Landroid/content/Context;)V

    iput-object v0, p0, Lcom/robtopx/geometryjump/GeometryJump;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    iget-object v0, p0, Lcom/robtopx/geometryjump/GeometryJump;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v1, v0}, Landroid/widget/FrameLayout;->addView(Landroid/view/View;)V

    iget-object v0, p0, Lcom/robtopx/geometryjump/GeometryJump;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    const/4 v3, 0x2

    invoke-virtual {v0, v3}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setEGLContextClientVersion(I)V

    iget-object v0, p0, Lcom/robtopx/geometryjump/GeometryJump;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    new-instance v3, Lorg/cocos2dx/lib/Cocos2dxRenderer;

    invoke-direct {v3}, Lorg/cocos2dx/lib/Cocos2dxRenderer;-><init>()V

    invoke-virtual {v0, v3}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setCocos2dxRenderer(Lorg/cocos2dx/lib/Cocos2dxRenderer;)V

    iget-object v0, p0, Lcom/robtopx/geometryjump/GeometryJump;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v0, v2}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setTextField(Lorg/cocos2dx/lib/Cocos2dxEditText;)V

    invoke-virtual {v2, v4}, Lorg/cocos2dx/lib/Cocos2dxEditText;->setInputType(I)V

    invoke-virtual {p0, v1}, Lcom/robtopx/geometryjump/GeometryJump;->setContentView(Landroid/view/View;)V

    :goto_1
    new-instance v0, Lcom/customRobTop/GameAdManagerCB;

    invoke-direct {v0}, Lcom/customRobTop/GameAdManagerCB;-><init>()V

    sput-object v0, Lcom/robtopx/geometryjump/GeometryJump;->GAM_:Lcom/customRobTop/GameAdManagerCB;

    sget-object v0, Lcom/robtopx/geometryjump/GeometryJump;->GAM_:Lcom/customRobTop/GameAdManagerCB;

    const-string v1, ""

    const-string v2, "51cd781316ba47c20d000005"

    const-string v3, "7acb353a5aea7419450cd8c0a7d9b67face1f6fa"

    invoke-virtual {v0, p0, v1, v2, v3}, Lcom/customRobTop/GameAdManagerCB;->setup(Lcom/customRobTop/BaseRobTopActivity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V

    return-void

    :cond_0
    sget v0, Landroid/os/Build$VERSION;->SDK_INT:I

    const/16 v1, 0x9

    if-lt v0, v1, :cond_1

    const/4 v0, 0x6

    invoke-virtual {p0, v0}, Lcom/robtopx/geometryjump/GeometryJump;->setRequestedOrientation(I)V

    goto :goto_0

    :cond_1
    const/4 v0, 0x0

    invoke-virtual {p0, v0}, Lcom/robtopx/geometryjump/GeometryJump;->setRequestedOrientation(I)V

    goto :goto_0

    :cond_2
    const-string v0, "activity"

    const-string v1, "don\'t support gles2.0"

    invoke-static {v0, v1}, Landroid/util/Log;->d(Ljava/lang/String;Ljava/lang/String;)I

    invoke-virtual {p0}, Lcom/robtopx/geometryjump/GeometryJump;->finish()V

    goto :goto_1
.end method

.method protected onPause()V
    .locals 1

    invoke-super {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onPause()V

    iget-object v0, p0, Lcom/robtopx/geometryjump/GeometryJump;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->onPause()V

    return-void
.end method

.method protected onResume()V
    .locals 1

    invoke-super {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onResume()V

    iget-object v0, p0, Lcom/robtopx/geometryjump/GeometryJump;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->onResume()V

    return-void
.end method

.method public onStart()V
    .locals 0

    invoke-super {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onStart()V

    return-void
.end method

.method public onStop()V
    .locals 0

    invoke-super {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onStop()V

    return-void
.end method
