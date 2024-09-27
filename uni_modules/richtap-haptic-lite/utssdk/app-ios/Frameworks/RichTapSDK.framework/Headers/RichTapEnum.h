//
//  RichTapEnum.h
//  RichTapSDK
//
//  Created by 60057362 on 2022/3/28.
//



typedef NS_ENUM(NSUInteger, HAPTIC_MODE){
    MUSIC_LIVE,
    MUSIC_POWER,
    MUSIC_RHYTHM
};

typedef NS_ENUM(NSUInteger, PrebakedEffectId){
    RT_CLICK,
    RT_DOUBLE_CLICK,
    RT_SOFT_CLICK,
    RT_TICK,
    RT_THUD,
    RT_FAILURE,
    RT_SUCCESS,
    RT_RAMP_UP,
    RT_TOGGLE_SWITCH,
    RT_LONG_PRESS,
    RT_VIRTUAL_KEY,
    RT_KEYBOARD_TAP,
    RT_CLOCK_TICK,
    RT_CALENDAR_DATE,
    RT_CONTEXT_CLICK,
    RT_KEYBOARD_RELEASE,
    RT_VIRTUAL_KEY_RELEASE,
    RT_TEXT_HANDLE_MOVE,
    RT_ENTRY_BUMP,
    RT_DRAG_CROSSING,
    RT_GESTURE,
    RT_CONFIRM,
    RT_REJECT,
    RT_BOMB,
    RT_SWORD,
    RT_GUNSHOT,
    RT_SPEED_UP,
    RT_JUMP,
    RT_DRUM,
    RT_COIN_DROP,
    RT_HEARTBEAT,
    RT_PLUCKING,
    RT_DRAWING_ARROW,
    RT_CAMERA_SHUTTER,
    RT_FIREWORKS,
    RT_SNIPER_RIFLE,
    RT_ASSAULT_RIFLE,
    RT_CYMBAL,
    RT_TAMBOURINE,
    RT_FAST_MOVING,
    RT_FLY,
    RT_FOOTSTEP,
    RT_ICE,
    RT_LIGHTNING,
    RT_SPRING,
    RT_SWING,
    RT_WIND,
    RT_VICTORY,
    RT_AWARD,
    RT_GAMEOVER
};

static NSErrorDomain RichTapErrorDomain = @"com.RichTap.CoreHaptics";
typedef NS_ERROR_ENUM(RichTapErrorDomain, RichTapError) {

    ErrorUnknow                             = -1,
    ErrorParameter                          = 100,
    ErrorParseJson                          = 101,
    ErrorHeVersionMismatch                  = 102,
    ErrorCurrentNoPlaying                   = 103,
    ErrorCurrentNoHapticStop                = 104,
    ErrorCurrentHapticAlreadyStop           = 105,
    ErrorCurrentHapticAlreadyPlaying        = 106,
    ErrorSystemVersionNotSupported          = 110,
    ErrorCurrentPlaybackInterrupted         = 111,
};



//[9,8,7,6,5,4,3,2,1];
//[8,7,6,5,4,3,2,1,9];
//[8,7,9,6,5,4,3,2,1];
