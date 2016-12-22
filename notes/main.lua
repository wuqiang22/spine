
cc.FileUtils:getInstance():setPopupNotify(false)
cc.FileUtils:getInstance():addSearchPath("src/")
cc.FileUtils:getInstance():addSearchPath("res/")

require "config"
require "cocos.init"

require "cocos.spine.SpineConstants"


local function testSpine(scene)
    local skeletonNode = sp.SkeletonAnimation:create("spineboy.json", "spineboy.atlas", 0.6)
    
    
    skeletonNode:registerSpineEventHandler(function(event)
        print(string.format("[spine] %d start: %s,%s,%s", 
            event.trackIndex,
            event.animation,
            event.type,
            event.loopCount))
    end,sp.EventType.ANIMATION_START)
    
    skeletonNode:registerSpineEventHandler(function(event)
        print(string.format("[spine] %d end: %s,%s,%s", 
            event.trackIndex,
            event.animation,
            event.type,
            event.loopCount))
    end,sp.EventType.ANIMATION_END)
    
    skeletonNode:registerSpineEventHandler(function(event)
        print(string.format("[spine] %d compelete: %s,%s,%s", 
            event.trackIndex,
            event.animation,
            event.type,
            event.loopCount))
    end,sp.EventType.ANIMATION_COMPLETE)
    
    skeletonNode:registerSpineEventHandler(function(event)
        print(string.format("[spine] %d event: %s, %d, %f, %s", 
            event.trackIndex,
            event.eventData.name,
            event.eventData.intValue,
            event.eventData.floatValue,
            event.eventData.stringValue)) 
    end,sp.EventType.ANIMATION_EVENT)
    
    
  --  skeletonNode:setAnimation(1, "hit", true)
    skeletonNode:setAnimation(2, "walk", false)
   -- skeletonNode:setAnimation(1, "death", true)
    skeletonNode:addAnimation(2,"death",true,0)
   -- skeletonNode:setAnimation(1, "death", true)
 --   skeletonNode:setAnimation(1, "hit", true)
  --  skeletonNode:setMix("walk","hit",0.2)
  --  skeletonNode:setAnimation(5,"hit",false)
  --  skeletonNode:setMix()
   -- skeletonNode:setAnimation(1, "hit", true)
   skeletonNode:setMix("walk","death",1)
    scene:addChild(skeletonNode)
    
    local size = cc.Director:getInstance():getWinSize()
    skeletonNode:setPosition(cc.p(size.width/2,0))
end

local function main()
   -- require("app.MyApp"):create():run()
   local scene = cc.Scene:create()
    cc.Director:getInstance():replaceScene(scene)
   
   
   if true then
	testSpine(scene)
	return
   end

  --  local container = cc.Sprite:create("PlayButton.png")
  --  container:addChild(sprite1)
  --  container:addChild(sprite2)

 --   local spriteBatchNode = cc.SpriteBatchNode:create("PlayButton.png")
    local sprite1 = cc.Sprite:create("PlayButton.png")
    local sprite2 = cc.Sprite:create("PlayButton.png")
    local container = cc.Sprite:create("PlayButton.png")
    container:addChild(sprite1)
    sprite1:setLocalZOrder(101)
    container:addChild(sprite2)
    sprite1:setPosition(cc.p(500,500))
    sprite2:setPosition(cc.p(600,530))
  --  sprite1:setLocalZOrder(101)
  --  sprite2:setLocalZOrder(100)

    local spriteBatchNode = cc.SpriteBatchNode:create("PlayButton.png")
    scene:addChild(spriteBatchNode)
    spriteBatchNode:addChild(container)

    

    
    
    local listener = cc.EventListenerTouchOneByOne:create()
    local function onTouchesBegan(touch, event)
    --    sprite1:setLocalZOrder(102)
    --    sprite2:setLocalZOrder(101)
        return true
    end
    
    listener:registerScriptHandler(onTouchesBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    
    listener:setSwallowTouches(false)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener,  1000)

end




__G__TRACKBACK__ = function(msg)
    local message = msg
    local msg = debug.traceback(3)
    release_print(msg)
 --   buglyReportLuaException(tostring(message), debug.traceback())
    return message
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    release_print(msg)
end



