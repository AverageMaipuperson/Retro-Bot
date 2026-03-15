.class Lorg/cocos2dx/lib/n;
.super Ljava/lang/Object;

# interfaces
.implements Ljava/lang/Runnable;


# instance fields
.field final synthetic a:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

.field private final synthetic b:[I

.field private final synthetic c:[F

.field private final synthetic d:[F


# direct methods
.method constructor <init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;[I[F[F)V
    .locals 0

    iput-object p1, p0, Lorg/cocos2dx/lib/n;->a:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    iput-object p2, p0, Lorg/cocos2dx/lib/n;->b:[I

    iput-object p3, p0, Lorg/cocos2dx/lib/n;->c:[F

    iput-object p4, p0, Lorg/cocos2dx/lib/n;->d:[F

    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method


# virtual methods
.method public run()V
    .locals 4

    iget-object v0, p0, Lorg/cocos2dx/lib/n;->a:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-static {v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->access$3(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)Lorg/cocos2dx/lib/Cocos2dxRenderer;

    move-result-object v0

    iget-object v1, p0, Lorg/cocos2dx/lib/n;->b:[I

    iget-object v2, p0, Lorg/cocos2dx/lib/n;->c:[F

    iget-object v3, p0, Lorg/cocos2dx/lib/n;->d:[F

    invoke-virtual {v0, v1, v2, v3}, Lorg/cocos2dx/lib/Cocos2dxRenderer;->handleActionMove([I[F[F)V

    return-void
.end method
