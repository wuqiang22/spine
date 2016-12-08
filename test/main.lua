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
 
  skeletonNode:setAnimation(2,"shoot",true)  --���Թر�������ԣ������Ƿ�ԭΪԭ����attachment

   skeletonNode:setDebugSlotsEnabled(false)
   textureName = "122.png"
   local sprite = cc.Sprite:create(textureName)	--��ʽһ��ֱ����Դ�ļ�����
 --  local sprite = cc.Sprite:createWithSpriteFrame(spriteFrame)   --��ʽ������֡�ķ�ʽ
   sprite:retain()

   local isChanging = false
   local btnStart = rootNode:getChildByName("btn_start")
   if btnStart then
      btnStart:addClickEventListener(function(sender)
        if isChanging then
          return
        end
        isChanging = true
		--��ʼ�滻������һ���Ѿ�����
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
          skeletonNode:clearAllRepleacedAttachments()   --��ԭ
      end)
   end
end


