.class Lorg/cocos2dx/lib/a;
.super Landroid/os/Handler;


# instance fields
.field final synthetic a:Lorg/cocos2dx/lib/Cocos2dxActivity;


# direct methods
.method constructor <init>(Lorg/cocos2dx/lib/Cocos2dxActivity;)V
    .locals 0

    iput-object p1, p0, Lorg/cocos2dx/lib/a;->a:Lorg/cocos2dx/lib/Cocos2dxActivity;

    invoke-direct {p0}, Landroid/os/Handler;-><init>()V

    return-void
.end method


# virtual methods
.method public handleMessage(Landroid/os/Message;)V
    .locals 3

    iget v0, p1, Landroid/os/Message;->what:I

    packed-switch v0, :pswitch_data_0

    :goto_0
    return-void

    :pswitch_0
    iget-object v1, p0, Lorg/cocos2dx/lib/a;->a:Lorg/cocos2dx/lib/Cocos2dxActivity;

    iget-object v0, p1, Landroid/os/Message;->obj:Ljava/lang/Object;

    check-cast v0, Lorg/cocos2dx/lib/s;

    iget-object v2, v0, Lorg/cocos2dx/lib/s;->a:Ljava/lang/String;

    iget-object v0, p1, Landroid/os/Message;->obj:Ljava/lang/Object;

    check-cast v0, Lorg/cocos2dx/lib/s;

    iget-object v0, v0, Lorg/cocos2dx/lib/s;->b:Ljava/lang/String;

    invoke-static {v1, v2, v0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->access$0(Lorg/cocos2dx/lib/Cocos2dxActivity;Ljava/lang/String;Ljava/lang/String;)V

    goto :goto_0

    :pswitch_data_0
    .packed-switch 0x1
        :pswitch_0
    .end packed-switch
.end method
