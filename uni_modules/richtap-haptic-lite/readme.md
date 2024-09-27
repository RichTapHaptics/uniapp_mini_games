# RichTap Haptics Lite

*为第三方应用开发者提供的振动接口，能极大的方便开发者应用振动效果，配合振动效果文件，充分发挥硬件性能，提升体验。*

## 使用说明

下面为`uniappx`和`vue3`环境的调用示例

```JavaScript
<template>
</template>

<script setup lang="uts">
	import { initialize, InitializeParams, quit, QuitParams, playHaptic, PlayHapticParams, stopHaptic, StopHapticParams, updateLoopParameters, UpdateLoopParametersParams, playExtPrebaked, PlayExtPrebakedParams } from '@/uni_modules/richtap-haptic-lite'
	
	const he = '{\"Metadata\":{\"Created\":\"2024-09-11\",\"Description\":\"Exported from RichTap Creator Pro\",\"Version\":2},\"PatternList\":[{\"AbsoluteTime\":94,\"Pattern\":[{\"Event\":{\"Type\":\"transient\",\"RelativeTime\":0,\"Parameters\":{\"Intensity\":78,\"Frequency\":50},\"Index\":0}}]}]}'
	
	onMounted(() => {
		// 可以在页面打开后初始化插件
		initialize({
			fail: (err) => {
				console.log(err)
			}
		} as InitializeParams)
	})
	
	onUnload(() => {
		// 可以在页面卸载时释放插件资源
		quit({
			fail: (err) => {
				console.log(err)
			}
		} as QuitParams)
	})
	
	const play = (): void => {
		playHaptic({
			heStr: he,
			loop: 2,
			interval: 100,
			intensity: 255,
			frequency: 100,
			success: () => {
				console.log('调用成功')
			},
			fail: (err) => {
				console.log(err)
			}
		} as PlayHapticParams)
	}
	
	const stop = (): void => {
		stopHaptic({
			success: () => {
				console.log('调用成功')
			},
			fail: (err) => {
				console.log(err)
			}
		} as StopHapticParams)
	}
	
	const setLoop = (): void => {
		updateLoopParameters({
			intensity: 255,
			interval: 0,
			frequency 100,
			success: () => {
				console.log('调用成功')
			},
			fail: (err) => {
				console.log(err)
			}
		} as UpdateLoopParametersParams)
	}

	const playPrebaked = () => {
		playExtPrebaked({
			effectName: 'RT_BOMB',
			intensity: 255
		} as PlayExtPrebakedParams)
	}
	
</script>

<style>
</style>

```

下面为`uniapp`和`vue2`环境的调用示例

```JavaScript
<template>
</template>

<script>
	import { initialize, quit, playHaptic, stopHaptic, updateLoopParameters, playExtPrebaked } from '@/uni_modules/richtap-haptic-lite'
	
	export default {
		data () {
			return {
				he: '{\"Metadata\":{\"Created\":\"2024-09-11\",\"Description\":\"Exported from RichTap Creator Pro\",\"Version\":2},\"PatternList\":[{\"AbsoluteTime\":94,\"Pattern\":[{\"Event\":{\"Type\":\"transient\",\"RelativeTime\":0,\"Parameters\":{\"Intensity\":78,\"Frequency\":50},\"Index\":0}}]}]}',
			}
		},
		mounted() {
			// 可以在页面打开后初始化插件
			initialize({
				fail: (err) => {
					console.log(err)
				}
			})
		},
		onUnload() {
			// 可以在页面卸载时释放插件资源
			quit({
				fail: (err) => {
					console.log(err)
				}
			})
		}
		methods: {
			playPrebaked () {
				playExtPrebaked({
					effectName: 'RT_BOMB',
					intensity: 255
				})
			},
			play () {
				playHaptic({
					heStr: this.he,
					loop: 2,
					interval: 100,
					intensity: 255,
					frequency: 100,
					success: () => {
						console.log('调用成功')
					},
					fail: (err) => {
						console.log(err)
					}
				})
			},
			stop () {
				stopHaptic({
					success: () => {
						console.log('调用成功')
					},
					fail: (err) => {
						console.log(err)
					}
				})
			},
			setLoop () {
				updateLoopParameters({
					interval: 0,
					intensity: 255,
					frequency: 100,
					success: () => {
						console.log('调用成功')
					},
					fail: (err) => {
						console.log(err)
					}
				})
			}
		}
	}
</script>

<style>
</style>

```

## API

### 获取SDK版本号

`getSDKVersion(options: GetSDKVersionParams): void`
获取RichTap SDK的版本号

#### 参数说明

**options.success**, Type: (res: string) => void, 成功回调，返回RichTap SDK的版本号  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 获取插件版本号

`getPluginVersion(options: GetPluginVersionParams): void`

获取本插件的版本号

#### 参数说明

**options.success**, Type: (res: string) => void, 成功回调，返回本插件的版本号  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 检查当前设备的RichTap振动能力

`isRichTapSupported(options): void`

当前系统是否支持[高品质振动](https://www.richtap-haptics.com/explore/hapticslist)

#### 参数说明

**options.success**, Type: (res: boolean) => void, 成功回调，返回`true`表示当前设备支持RichTap高品质振动，`flase`为不支持RichTap高品质振动（但仍会以系统兼容方式振动）  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 初始化插件

`initialize(options: InitializeParams): void`

初始化插件，振动之前必须初始化，否则无法振动（iOS除外）

#### 参数说明

**options.success**, Type: () => void, 成功回调，可选参数  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 释放/退出插件

`quit(options: QuitParams): void`

App退出时释放RichTap资源

#### 参数说明

**options.success**, Type: () => void, 成功回调，可选参数  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 播放振动效果

`playHaptic(options: PlayHapticParams): void`

播放振动效果(HE)，**iOS平台不支持`interval`和`frequency`参数**

#### 参数说明
**options.heStr**, Type: string, 振动效果HE文件内容，是一个JSON字符串  
**options.loop**, Type: number, 循环次数，取值范围是大于等于`-1`，`-1`表示无限循环，0表示只播放一次（不循环）  
**options.interval**, Type: number, 循环播放情况下相邻两次播放之间的时间间隔，以毫秒为单位  
**options.intensity**, Type: number, 振动强度缩放值，取值范围是`[0, 511]`，`255`表示按HE原有设计强度振动  
**options.frequency**, Type: number, 变频值，取值范围`[-100, 100]`，`0`表示不对HE做变频  
**options.success**, Type: () => void, 成功回调，可选参数  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 动态调整振动效果参数

`updateLoopParameters(options: UpdateLoopParametersParams): void`

在播放过程中动态调整振动效果参数

#### 参数说明

**options.intensity**, Type: number, 振动强度缩放值，取值范围是`[0, 255]`  
**options.interval**, Type: number, 循环播放情况下相邻两次播放之间的时间间隔，以毫秒为单位  
**options.frequency**, Type: number, 变频值，取值范围`[-100, 100]`，`0`表示不对HE做变频  
**options.success**, Type: () => void, 成功回调，可选参数  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 停止振动

`stopHaptic(options: StopHapticParams): void`

停止所有振动效果

#### 参数说明

**options.success**, Type: () => void, 成功回调，可选参数  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

### 播放预置振动效果

`PlayExtPrebaked(options: PlayExtPrebakedParams): void`

播放插件预置效果库中的振动效果

#### 参数说明

**options.effectName** Type: PrebakedEffectName, 预置效果名称  
**options.intensity**, Type: number,  强度缩放值 取值范围是[0, 255]，255表示100%增益  
**options.success**, Type: () => void, 成功回调，可选参数  
**options.fail**，Type: (res : HapticError) => void, 错误回调，可选参数  

## 数据类型

### 预置振动效果

在播放插件预置效果时，参数`effectName`的数据类型，提供如下50种振动效果

```typescript
export type PrebakedEffectName = 'RT_CLICK' | 'RT_GUNSHOT' | 'RT_DOUBLE_CLICK' | 'RT_SPEED_UP' | 'RT_SOFT_CLICK' | 'RT_JUMP' | 'RT_TICK' | 'RT_DRUM' | 'RT_THUD' | 'RT_COIN_DROP' | 'RT_FAILURE' | 'RT_HEARTBEAT' | 'RT_SUCCESS' | 'RT_PLUCKING' | 'RT_RAMP_UP'	| 'RT_DRAWING_ARROW' | 'RT_TOGGLE_SWITCH'	| 'RT_CAMERA_SHUTTER' | 'RT_LONG_PRESS'	| 'RT_FIREWORKS' | 'RT_VIRTUAL_KEY'	| 'RT_SNIPER_RIFLE' | 'RT_KEYBOARD_TAP'	| 'RT_ASSAULT_RIFLE' | 'RT_CLOCK_TICK'	| 'RT_CYMBAL' | 'RT_CALENDAR_DATE' | 'RT_TAMBOURINE' | 'RT_CONTEXT_CLICK' | 'RT_FAST_MOVING' | 'RT_KEYBOARD_RELEASE' | 'RT_FLY' | 'RT_VIRTUAL_KEY_RELEASE'	| 'RT_FOOTSTEP' | 'RT_TEXT_HANDLE_MOVE'	| 'RT_ICE' | 'RT_ENTRY_BUMP' | 'RT_LIGHTNING' | 'RT_DRAG_CROSSING' | 'RT_SPRING' | 'RT_GESTURE'	| 'RT_SWING' | 'RT_CONFIRM'	| 'RT_WIND' | 'RT_REJECT'	| 'RT_VICTORY' | 'RT_BOMB' | 'RT_AWARD' | 'RT_SWORD'	| 'RT_GAMEOVER'

```

### 错误对象

插件导出的每个方法都有fail参数（可选），会返回错误信息

```typescript
export class HapticError extends UniError {
  constructor(errCode: HapticErrorCode, cause: Error | null = null) {
    super();
    this.errSubject = HapticErrorSubject;
    this.errCode = errCode;
    this.errMsg = HapticErrorInfo.get(errCode) ?? "";
		this.cause = cause;
  }
}

```

## HE文件

可以通过[RichTap Haptics 设计大师套件](https://www.richtap-haptics.com/product/creator)制作并导出HE振动文件