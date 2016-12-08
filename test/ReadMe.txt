实现的效果：给一个Sprite，无论它是如何创建的(原图/精灵帧)，都可以将它替换到spine的一个节点上。

spine的显示是渲染所有的slot的attachment,所以我们可以通过解析Sprite的信息来创建新的attachment,替换原来的slot的
attachment就好了。

为了可以还原以前的效果，所以替换的时候，并不能将以前的attachment dispose掉，而是保存起来。

所有的attachment保存在_spSkin下，以_Entry的方式保存,_spSkin下的entries，所以我们在 _spSkin下再创建一个beRepleacedEnties
用来保存被替换掉的Entry,为了好搜索，在源码的基础上给Entry加了pre属性，使单向链表变成双向链表
详情请见：spSkin.c


新的attachment是需要使用oldAttachment的一些信息的。比如初始的旋转，位置，缩放等等，见json文件的skin-slot-attachment配置
当然还要初始化新的region到attachment中，新的region中包括Sprite纹理信息

