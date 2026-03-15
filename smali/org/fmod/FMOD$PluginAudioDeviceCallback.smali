.class Lorg/fmod/FMOD$PluginAudioDeviceCallback;
.super Landroid/media/AudioDeviceCallback;
.source "FMOD.java"


# annotations
.annotation system Ldalvik/annotation/EnclosingClass;
    value = Lorg/fmod/FMOD;
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0x8
    name = "PluginAudioDeviceCallback"
.end annotation


# static fields
.field private static deviceSet:Ljava/util/HashSet;
    .annotation system Ldalvik/annotation/Signature;
        value = {
            "Ljava/util/HashSet<",
            "Ljava/lang/Integer;",
            ">;"
        }
    .end annotation
.end field


# direct methods
.method public constructor <init>([Landroid/media/AudioDeviceInfo;)V
    .locals 3

    .line 251
    invoke-direct {p0}, Landroid/media/AudioDeviceCallback;-><init>()V

    .line 252
    new-instance v0, Ljava/util/HashSet;

    invoke-direct {v0}, Ljava/util/HashSet;-><init>()V

    sput-object v0, Lorg/fmod/FMOD$PluginAudioDeviceCallback;->deviceSet:Ljava/util/HashSet;

    .line 254
    invoke-static {p1}, Lorg/fmod/FMOD;->access$100([Landroid/media/AudioDeviceInfo;)[Landroid/media/AudioDeviceInfo;

    move-result-object p1

    .line 255
    const/4 v0, 0x0

    :goto_0
    array-length v1, p1

    if-ge v0, v1, :cond_0

    .line 257
    sget-object v1, Lorg/fmod/FMOD$PluginAudioDeviceCallback;->deviceSet:Ljava/util/HashSet;

    aget-object v2, p1, v0

    invoke-virtual {v2}, Landroid/media/AudioDeviceInfo;->getId()I

    move-result v2

    invoke-static {v2}, Ljava/lang/Integer;->valueOf(I)Ljava/lang/Integer;

    move-result-object v2

    invoke-virtual {v1, v2}, Ljava/util/HashSet;->add(Ljava/lang/Object;)Z

    .line 255
    add-int/lit8 v0, v0, 0x1

    goto :goto_0

    .line 259
    :cond_0
    return-void
.end method


# virtual methods
.method public onAudioDevicesAdded([Landroid/media/AudioDeviceInfo;)V
    .locals 5

    .line 264
    nop

    .line 265
    nop

    .line 267
    invoke-static {p1}, Lorg/fmod/FMOD;->access$100([Landroid/media/AudioDeviceInfo;)[Landroid/media/AudioDeviceInfo;

    move-result-object p1

    .line 268
    const/4 v0, 0x0

    const/4 v1, 0x0

    const/4 v2, 0x0

    :goto_0
    array-length v3, p1

    if-ge v0, v3, :cond_3

    .line 270
    sget-object v3, Lorg/fmod/FMOD$PluginAudioDeviceCallback;->deviceSet:Ljava/util/HashSet;

    aget-object v4, p1, v0

    invoke-virtual {v4}, Landroid/media/AudioDeviceInfo;->getId()I

    move-result v4

    invoke-static {v4}, Ljava/lang/Integer;->valueOf(I)Ljava/lang/Integer;

    move-result-object v4

    invoke-virtual {v3, v4}, Ljava/util/HashSet;->contains(Ljava/lang/Object;)Z

    move-result v3

    if-nez v3, :cond_2

    .line 272
    aget-object v3, p1, v0

    invoke-virtual {v3}, Landroid/media/AudioDeviceInfo;->isSource()Z

    move-result v3

    const/4 v4, 0x1

    if-eqz v3, :cond_0

    .line 274
    const/4 v1, 0x1

    .line 276
    :cond_0
    aget-object v3, p1, v0

    invoke-virtual {v3}, Landroid/media/AudioDeviceInfo;->isSink()Z

    move-result v3

    if-eqz v3, :cond_1

    .line 278
    const/4 v2, 0x1

    .line 280
    :cond_1
    sget-object v3, Lorg/fmod/FMOD$PluginAudioDeviceCallback;->deviceSet:Ljava/util/HashSet;

    aget-object v4, p1, v0

    invoke-virtual {v4}, Landroid/media/AudioDeviceInfo;->getId()I

    move-result v4

    invoke-static {v4}, Ljava/lang/Integer;->valueOf(I)Ljava/lang/Integer;

    move-result-object v4

    invoke-virtual {v3, v4}, Ljava/util/HashSet;->add(Ljava/lang/Object;)Z

    .line 268
    :cond_2
    add-int/lit8 v0, v0, 0x1

    goto :goto_0

    .line 284
    :cond_3
    if-eqz v1, :cond_4

    .line 286
    invoke-static {}, Lorg/fmod/FMOD;->access$200()V

    .line 288
    :cond_4
    if-eqz v2, :cond_5

    .line 290
    invoke-static {}, Lorg/fmod/FMOD;->access$300()V

    .line 292
    :cond_5
    return-void
.end method

.method public onAudioDevicesRemoved([Landroid/media/AudioDeviceInfo;)V
    .locals 5

    .line 297
    nop

    .line 298
    nop

    .line 300
    invoke-static {p1}, Lorg/fmod/FMOD;->access$100([Landroid/media/AudioDeviceInfo;)[Landroid/media/AudioDeviceInfo;

    move-result-object p1

    .line 301
    const/4 v0, 0x0

    const/4 v1, 0x0

    const/4 v2, 0x0

    :goto_0
    array-length v3, p1

    if-ge v0, v3, :cond_3

    .line 303
    sget-object v3, Lorg/fmod/FMOD$PluginAudioDeviceCallback;->deviceSet:Ljava/util/HashSet;

    aget-object v4, p1, v0

    invoke-virtual {v4}, Landroid/media/AudioDeviceInfo;->getId()I

    move-result v4

    invoke-static {v4}, Ljava/lang/Integer;->valueOf(I)Ljava/lang/Integer;

    move-result-object v4

    invoke-virtual {v3, v4}, Ljava/util/HashSet;->contains(Ljava/lang/Object;)Z

    move-result v3

    if-eqz v3, :cond_2

    .line 305
    aget-object v3, p1, v0

    invoke-virtual {v3}, Landroid/media/AudioDeviceInfo;->isSource()Z

    move-result v3

    const/4 v4, 0x1

    if-eqz v3, :cond_0

    .line 307
    const/4 v1, 0x1

    .line 309
    :cond_0
    aget-object v3, p1, v0

    invoke-virtual {v3}, Landroid/media/AudioDeviceInfo;->isSink()Z

    move-result v3

    if-eqz v3, :cond_1

    .line 311
    const/4 v2, 0x1

    .line 313
    :cond_1
    sget-object v3, Lorg/fmod/FMOD$PluginAudioDeviceCallback;->deviceSet:Ljava/util/HashSet;

    aget-object v4, p1, v0

    invoke-virtual {v4}, Landroid/media/AudioDeviceInfo;->getId()I

    move-result v4

    invoke-static {v4}, Ljava/lang/Integer;->valueOf(I)Ljava/lang/Integer;

    move-result-object v4

    invoke-virtual {v3, v4}, Ljava/util/HashSet;->remove(Ljava/lang/Object;)Z

    .line 301
    :cond_2
    add-int/lit8 v0, v0, 0x1

    goto :goto_0

    .line 317
    :cond_3
    if-eqz v1, :cond_4

    .line 319
    invoke-static {}, Lorg/fmod/FMOD;->access$200()V

    .line 321
    :cond_4
    if-eqz v2, :cond_5

    .line 323
    invoke-static {}, Lorg/fmod/FMOD;->access$300()V

    .line 325
    :cond_5
    return-void
.end method
