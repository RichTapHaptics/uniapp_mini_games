//
//  AACHapticUtils.h
//  iOS_AAC_RichTap
//
//  Created by AAC Acoustic Technologies (Shanghai) Co., Ltd. on 2020/7/23.
//  Copyright © 2020 AAC Acoustic Technologies (Shanghai) Co., Ltd.. All rights reserved.
//

#import <RichTapSDK/RichTapEnum.h>
NS_ASSUME_NONNULL_BEGIN


@interface RichTapHapticUtils : NSObject

/// 查看是否支持core haptic
+(BOOL)supportRichTap;

/// 退出core haptic
/// @param error 如果quit操作失败，将返回错误信息
+(void)quit:(NSError *_Nullable *_Nullable)error;

/// 停止震动
/// @param error 如果stop操作失败，将返回错误信息
+(void)stop:(NSError *_Nullable *_Nullable)error;

/// 播放he2.0效果文件，声音与震动自动同步接口（适配高自定义场景）
/// @param str 文件路径或he格式json字符串
/// @param amplitude 播放强度，取值范围 [0-255]
/// @param freq  播放频率，取值范围 [0-100]（目前没影响，可传0）
/// @param playProgress 同步的callBack，内部return 播放器进度（单位秒）
/// @param error 如果playHaptic操作失败，将返回错误信息
+(void)playHaptic:(NSString*)str amplitude:(int)amplitude freq:(int)freq playProgress:(double(^_Nonnull)(void))playProgress error:(NSError *_Nullable *_Nullable)error;

/// 调用通用效果库中的效果,目前定义了49个效果
/// @param effect 指定哪个效果，取值范围[0,49]
/// @param strength 指定效果的强度，取值范围是[0, 255]，255表示100%强度
/// @param error 如果playExtPreBaked操作失败，将返回错误信息
+(void)playExtPreBaked:(PrebakedEffectId)effect strength:(int)strength error:(NSError *_Nullable *_Nullable)error;

/// 播放自定义的he基频信号
/// @param intensity 强度，取值范围 [0-100]
/// @param frequency 频率，取值范围 [0-100]
/// @param error 如果playExtPrebakedForHe操作失败，将返回错误信息
+(void)playExtPrebakedForHe:(int)intensity frequency:(int)frequency error:(NSError *_Nullable *_Nullable)error;


/// 播放he效果文件
/// @param str 文件路径或he格式json字符串
/// @param loop 文件循环次数，取值范围 [-1-+∞]
/// @param error 如果playHaptic操作失败，将返回错误信息
+(void)playHaptic:(NSString*)str loop:(int)loop error:(NSError *_Nullable *_Nullable)error;

/// 播放he效果文件，添加强度参数
/// @param str  文件路径或he格式json字符串
/// @param loop 文件循环次数，取值范围 [-1-+∞]
/// @param amplitude 播放强度，取值范围 [0-255]
/// @param error 如果playHaptic操作失败，将返回错误信息
+(void)playHaptic:(NSString*)str loop:(int)loop amplitude:(int)amplitude error:(NSError *_Nullable *_Nullable)error;


/// 用来调整Loop Pattern的缩放幅度和间隔，必须在loop>0时，才有效
/// @param amplitude 表示缩放值，取值范围是[0，255]
/// @param interval 表示每次播放结束到下次开始的时间，取值范围是[0，1000]ms
/// @param error 如果sendLoopParameter操作失败，将返回错误信息
+(void)sendLoopParameter:(int)amplitude interval:(int)interval error:(NSError *_Nullable *_Nullable)error;


/// 用来调整Loop Pattern的缩放幅度、频率和间隔，必须在loop>0时，才有效
/// @param amplitude 表示缩放值，取值范围是[0，255]
/// @param interval 表示每次播放结束到下次开始的时间，取值范围是[0，1000]ms
/// @param freq 表示频率，取值范围是[-200，200]
/// @param error 如果sendLoopParameter操作失败，将返回错误信息
+(void)sendLoopParameter:(int)amplitude interval:(int)interval freq:(int)freq error:(NSError *_Nullable *_Nullable)error;

@end

NS_ASSUME_NONNULL_END
