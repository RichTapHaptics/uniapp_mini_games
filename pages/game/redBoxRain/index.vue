<template>
	<view class="content">

		<view class="start" v-if="gameStatus == 1">
			<view class="btn" @tap="start">
				开始游戏
			</view>
		</view>

		<view v-else>
			<view class="ready" ref="ready" v-if="readyStatus">
				{{ready}}
			</view>

			<view class="top">
				<view class="left">
					剩余时间：{{downTime}}
				</view>
				<view class="right">
					抢到红包：{{redNumber}}
				</view>
			</view>


			<view class="redBox" v-for="(item,index) in redBox" v-if="gameStatus == 2">
				<image class="red" :style="{
					width:item.width,
					left: item.left,
					transform: item.transform,
					animationDuration: item.animationDuration
				}" :data-index="index"  src="@/static/redBoxRain/close.png" mode="widthFix" @touchend="rob" v-if="item.status">
				</image>
				<image class="red" :style="{
					width:item.width,
					left: item.left,
					transform: item.transform,
					animationDuration: item.animationDuration
				}" src="@/static/redBoxRain/open.png" mode="widthFix" v-else>
				</image>
			</view>

			<view class="endBox" v-if="gameStatus == 3">
				<view class="head">
					游戏结束
				</view>
				<view class="info">
					<view class="star">
						<image class="star-item" v-for="item1 in stars" :src="item1" mode="widthFix"></image>
					</view>
					<view class="tips">
						本轮红包雨一共抢到{{redNumber}}个红包
					</view>
					
					<view class="btn" @tap="playAgain">
						再玩一次
					</view>
				</view>
			</view>
		</view>

	</view>
</template>

<script>
	import { playExtPrebaked, initialize, quit } from '@/uni_modules/richtap-haptic-lite'
	
	export default {
		data() {
			return {
				// 红包生产速度
				rate: 280,
				// 红包雨集合
				redBox: [],
				// ready状态
				readyStatus: false,
				ready: 'ready',
				// 游戏时长
				downTime: 30,
				// 游戏结束定时器
				endTimer: null,
				// 获得红包个数
				redNumber: 0,
				// 游戏状态
				gameStatus: 1,
				// 游戏评分
				stars: [
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png'
				]
			}
		},
		mounted() {
			console.log('redBoxRain mounted');
			initialize({
				fail: (err) => {
					console.log(err.errCode);
				}
			})
		},
		onUnload() {
			console.log('redBoxRain onUnload');
			quit({
				fail: (err) => {
					console.log(err.errCode);
				}
			})
		},
		methods: {
			playHaptic () {
				playExtPrebaked({
					effectName: 'RT_CLICK',
					intensity: 255,
					fail: (err) => {
						console.log(err.errCode);
					}
				})
			},
			start() {
				this.playHaptic()
				let that = this;
				that.gameStatus = 2;
				that.readyStatus = true;
				const innerAudioContext = uni.createInnerAudioContext();
				innerAudioContext.autoplay = true;
				innerAudioContext.src = '../../../static/redBoxRain/music.mp3';
				innerAudioContext.onPlay(() => {
					console.log('开始播放');
				});
				innerAudioContext.onError((res) => {
					console.log(res.errMsg);
					console.log(res.errCode);
				});

				let timer = setTimeout(function() {
					that.ready = 'Go';
					that.rain();
					that.down();
					let timer1 = setTimeout(function() {
						that.readyStatus = false
						clearInterval(timer1);
					}, 500)
					clearInterval(timer);
				}, 1000)
			},
			down() {
				let that = this;
				let downTimer = setInterval(function() {
					if (that.downTime == 0) {
						that.gameStatus = 3;
						let onNumber = Math.round(that.redNumber/(that.redBox.length/5));
						that.stars.fill('../../../static/redBoxRain/starOn.png', 0, onNumber);
						clearInterval(downTimer);
					} else {
						that.downTime--;
					}
				}, 1000)
			},
			rain() {
				let that = this;
				let win = 0;
				uni.getSystemInfo({
					success: function(res) {
						win = res.windowWidth
					}
				});
				//随机红包旋转角度
				let rotate = (parseInt(Math.random() * 90 - 45)) + "deg";
				//随机红包宽度
				let width = (parseInt(Math.random() * 50)) + 120;
				//动画时间
				let durTime = parseInt(Math.random() * 1.5) + 2 + 's';
				//随机红包偏移位置
				let left = parseInt(Math.random() * win);
				if (left < 100) {
					left = parseInt(Math.random() * 100) + 100
				} else if (left > (win - width)) {
					left = (win + 70)
				}

				that.redBox.push({
					status: true,
					width: width + 'rpx',
					left: left + 'rpx',
					transform: 'rotate(' + rotate + ')',
					animationDuration: durTime
				})

				// 多少时间游戏结束
				setTimeout(() => {
					clearTimeout(that.endTimer)
					return false
				}, that.downTime * 1000)

				// 红包密度
				that.endTimer = setTimeout(() => {

					that.rain();
				}, this.rate)
			},

			rob(event) {
				let index = event.currentTarget.dataset.index;
				
				if(this.redBox[index].status){
					this.playHaptic()
					this.redBox[index].status = false;
					this.redNumber++
				}
				
				event.preventDefault();
			},
			reset () {
				this.redBox = []
				this.downTime = 30
				this.ready = 'ready'
				this.redNumber = 0
				this.stars = [
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png',
					'../../../static/redBoxRain/starOff.png'
				]
			},
			playAgain () {
				this.reset()
				this.start()
			}
		}
	}
</script>

<style lang="scss">
	.content {
		margin: 0;
		padding: 0;
		position: relative;
		width: 100vw;
		height: 100vh;
		overflow: hidden !important;
		background-image: url(@/static/redBoxRain/gameBg.png);
		background-size: 100% 100%;
		background-repeat: no-repeat;

		.top {
			width: 100%;
			height: 100rpx;
			line-height: 100rpx;
			display: flex;
			justify-content: center;
			align-items: center;

			.left,
			.right {
				width: 50%;
				color: yellow;
				text-align: center;
			}
		}

		.start {
			width: 100%;
			height: 100vh;
			background-image: url(@/static/redBoxRain/bg.jpg);
			background-size: 100% 100%;
			background-repeat: no-repeat;

			.btn {
				position: absolute;
				bottom: 20%;
				left: 30%;
				width: 40%;
				height: 70rpx;
				color: yellow;
				text-align: center;
				line-height: 70rpx;
				border: 1rpx solid yellow;
				border-radius: 50rpx;
			}
		}

		.ready {
			position: fixed;
			top: 45%;
			color: yellow;
			font-size: 80rpx;
			font-weight: bold;
			width: 100%;
			text-align: center;
		}

		.redBox {
			position: absolute;
			top: 100rpx;

			.red {
				position: absolute;
				animation: rain 5s linear 1 forwards;
			}

		}

		.endBox {
			position: absolute;
			top: 30%;
			left: 15%;
			width: 70%;
			height: 40vh;
			z-index: 99;
			background-color: #fff;
			text-align: center;

			.head {
				width: 100%;
				height: 10vh;
				line-height: 10vh;
				font-size: 35rpx;
				font-weight: bold;
				color: red;
			}

			.info {
				.star {
					.star-item {
						width: 10%;
						display: inline-block;
					}
				}
				
				.tips{
					margin: 5vh auto;
				}
				
				.btn{
					width: 80%;
					margin: 1vh auto;
					height: 6vh;
					line-height: 6vh;
					background-color: yellow;
					border-radius: 40rpx;
					font-weight: bold;
					color: #646464;
				}
			}
		}

		@keyframes rain {
			0% {
				transform: translateY(0);
			}

			100% {
				transform: translateY(120vh);
			}
		}


	}
</style>