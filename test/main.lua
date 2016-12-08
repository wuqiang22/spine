local function testSpine(scene)
    local tt = cc.CSLoader:getInstance()
    local rootNode = cc.CSLoader:createNode("res/Layer.csb")
    scene:addChild(rootNode)

    local textureName ="plist/Plist.png";
    cc.SpriteFrameCache:getInstance():addSpriteFrames("plist/Plist.plist", textureName)
    local frameName = "pic/122.png"
    local spriteFrame = cc.SpriteFrameCache:getInstance():getSpriteFrame(frameName)
    local skeletonNode = sp.SkeletonAnimation:create("spine/spineboy.json", "spine/spineboy.atlas", 0.6)
    skeletonNode:registerSpineEventHandler(function (event)
      print(string.format("[spine] %d start: %s", 
                              event.trackIndex,
                              event.animation,
                              event.loopCount))
	end, sp.EventType.ANIMATION_START)

  skeletonNode:registerSpineEventHandler(function (event)
      print(string.format("[spine] %d end:", 
                                event.trackIndex,
                              event.animation,
                              event.loopCount))
  end, sp.EventType.ANIMATION_END)
    
  skeletonNode:registerSpineEventHandler(function (event)
      print(string.format("[spine] %d %s complete: %d", 
                              event.trackIndex,
                              event.animation,
                              event.loopCount))
  end, sp.EventType.ANIMATION_COMPLETE)
  local winSize = cc.Director:getInstance():getWinSize()
  skeletonNode:setPosition(cc.p(winSize.width/2,100))
  scene:addChild(skeletonNode)
 
  skeletonNode:setAnimation(2,"shoot",true)  --可以关闭这个试试，测试是否还原为原来的attachment

   skeletonNode:setDebugSlotsEnabled(false)
   textureName = "122.png"
   local sprite = cc.Sprite:create(textureName)	--方式一：直接用源文件创建
 --  local sprite = cc.Sprite:createWithSpriteFrame(spriteFrame)   --方式二：用帧的方式
   sprite:retain()

   local isChanging = false
   local btnStart = rootNode:getChildByName("btn_start")
   if btnStart then
      btnStart:addClickEventListener(function(sender)
        if isChanging then
          return
        end
        isChanging = true
		--开始替换，上面一种已经废弃
--        skeletonNode:repleaceAttachmentWithSpriteFrame("front_fist","front_fist_closed",spriteFrame,textureName,frameName)
          skeletonNode:repleaceAttachmentWithSprite("front_fist","front_fist_closed",sprite,textureName,textureName)
      end)
   end

   local btnEnd = rootNode:getChildByName("btn_end")
   if btnEnd then
      btnEnd:addClickEventListener(function(sender)
          if not isChanging then
            return
          end
          isChanging = false
          skeletonNode:clearAllRepleacedAttachments()   --还原
      end)
   end
end


