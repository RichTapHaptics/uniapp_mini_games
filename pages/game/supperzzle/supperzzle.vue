<template>
	<view class="content">
		<view class="start" v-if="gameStatus == 1">
			<view class="title">
				卡 牌 对 对 碰
			</view>
			<image class="icon" :class="max" src="@/static/supperzzle/card.png" mode="widthFix"></image>
			<view class="btn" @tap="start">
				开始游戏
			</view>
		</view>
		<view v-else-if="gameStatus == 2">
			
			<view class="down">
				倒计时：{{downTime}}s
			</view>
			<view class="box">
				<view style="width: 100%;" v-for="(item,index) in boxs" :key="index">
					<view v-if="item.status == 1" class="item1" :class="{'roll':item.roll,'die':item.die}"
						@tap="roll(index)"></view>
			
					<view v-else-if="item.status == 2" class="item2" :class="{'roll':item.restore,'die':item.die}"
						:style="{'background-image':`url(${item.bg})`,'background-size':'100% 100%'}" @tap="restore(index)">
					</view>
			
					<view class="item3" v-else>
						
					</view>
				</view>
			</view>
		</view>
		
		<view class="end" v-else>
			<image class="success" v-if="success" src="@/static/supperzzle/success.png" mode="widthFix"></image>
			<image class="error" v-else src="@/static/supperzzle/error.png" mode="widthFix"></image>
		
			<view class="again" :class="{'again1':success}" @tap="playAgain">{{success?'继续挑战':'不服再战'}}</view>
		</view>
		
		
	</view>
</template>

<script>
	import { playExtPrebaked, initialize, quit, playHaptic } from '@/uni_modules/richtap-haptic-lite'
	import he1 from '@/static/supperzzle/start.json'
	import he2 from '@/static/supperzzle/click.json'
	import he3 from '@/static/supperzzle/remove.json'
	
	export default {
		data() {
			return {
				// 卡片旋转
				// 消除卡片对
				dispel:0,
				// 挑战状态
				success:false,
				// 游戏状态 1未开始 2进行中 3结束
				gameStatus:1,
				// 游戏时间
				downTime:60,
				// icon变大动画
				max:'',
				// 翻转的卡牌下标
				check1: '',
				check2: '',
				boxs: [],
				clickHe: '',
				startHe: '',
				removeHe: ''
			};
		},
		mounted() {
			console.log('luckyGrid mounted');
			initialize({
				fail: (err) => {
					console.log(err.errCode);
				}
			})
			this.clickHe = JSON.stringify(he2)
			this.startHe = JSON.stringify(he1)
			this.removeHe = JSON.stringify(he3)
		},
		onLoad() {
			this.reset()
		},
		onUnload() {
			console.log('luckyGrid onUnload');
			quit({
				fail: (err) => {
					console.log(err.errCode);
				}
			})
		},
		methods: {
			playHaptic (effectName = 'RT_CLICK') {
				playExtPrebaked({
					effectName,
					intensity: 255,
					fail: (err) => {
						console.log(err.errCode);
					}
				})
			},
			playHe (he) {
				playHaptic({
					heStr: he,
					loop: 0,
					intensity: 255,
					interval: 0,
					frequency: 0,
				})
			},
			// 开始游戏
			start(){
				// this.playHaptic()
				let that = this;
				this.max = 'max';
				this.play();
				
				setTimeout(function(){
					that.gameStatus = 2;
					that.down();
				},2000)
			},
			// 翻转卡片
			roll(index) {
				let that = this;
				this.play1();
				if (this.check1 === '' || this.check2 === '') {
					this.check1 === '' ? this.check1 = index : this.check2 = index;

					this.boxs[index].roll = true;
					this.boxs[index].restore = false;
					setTimeout(function() {
						that.boxs[index].status = 2;
						that.check();
					}, 1000)


				} else {
					let a = this.check1;
					let b = this.check2;
					this.boxs[this.check1].roll = false;
					this.boxs[this.check1].restore = true;
					this.boxs[this.check2].roll = false;
					this.boxs[this.check2].restore = true;
					this.check1 = index;
					this.check2 = '';

					this.boxs[index].roll = true;
					this.boxs[index].restore = false;

					setTimeout(function() {
						that.boxs[a].status = 1;
						that.boxs[b].status = 1;
						that.boxs[index].status = 2;
					}, 1000)

					this.check1 = index;
					this.check2 = '';

				}
			},
			// 还原卡片
			restore(index) {
				let that = this;
				this.play1();
				this.check1 === index ? this.check1 = '' : this.check2 = '';
				this.boxs[index].restore = true;
				this.boxs[index].roll = false;

				setTimeout(function() {
					that.boxs[index].status = 1;
				}, 1000)
			},
			// 检查翻转的卡牌是否一致
			check() {
				let that = this;
				if (this.check1 === '' || this.check2 === '') {
					return
				}
				if (this.boxs[this.check1].value == this.boxs[this.check2].value) {
					// this.playHaptic('RT_AWARD')
					this.playHe(this.removeHe)
					let a = this.check1;
					let b = this.check2;
					this.boxs[this.check1].roll = false;
					this.boxs[this.check1].restore = false;
					this.boxs[this.check2].roll = false;
					this.boxs[this.check2].restore = false;
					this.boxs[this.check1].die = true;
					this.boxs[this.check2].die = true;
					this.check1 = '';
					this.check2 = '';
					this.dispel++;
					if(this.dispel == 6){
						setTimeout(function(){
							that.gameStatus = 3;
							that.success = true;
							console.log('游戏成功');
							that.playHaptic('RT_VICTORY')
						},800)
					}
					setTimeout(function() {
						that.boxs[a].status = 3;
						that.boxs[b].status = 3;
					},2000)
				}
			},
			// 游戏倒计时
			down(){
				let that = this;
				let downTimer = setInterval(function() {
					if (that.downTime == 0) {
						if(that.dispel == 6){
							that.success = true;
						}
						that.gameStatus = 3;
						console.log('游戏失败');
						that.playHaptic('_GAMEOVER')
						clearInterval(downTimer);
					} else {
						that.downTime--;
					}
				}, 1000)
			},
			
			reset () {
				this.dispel = 0
				this.success = false
				this.gameStatus = 1
				this.downTime = 60
				this.max = ''
				this.check1 = ''
				this.check2 = ''
				// 初始化卡牌
				this.resetBox()
				// 随机打乱排序
				this.boxs.sort(function() {
					return Math.random() - 0.5;
				})
			},
			
			playAgain () {
				this.reset()
				this.start()
			},
			
			play(){
				let innerAudioContext = uni.createInnerAudioContext();
				innerAudioContext.src = '../../../static/supperzzle/start.mp3';
				innerAudioContext.play();
				this.playHe(this.startHe)
			},
			play1(){
				let innerAudioContext = uni.createInnerAudioContext();
				innerAudioContext.src = '../../../static/supperzzle/click.mp3';
				innerAudioContext.play();
				this.playHe(this.clickHe)
			},
			resetBox () {
				this.boxs = [
					{
						// 翻转状态
						roll: false,
						// 还原状态
						restore: false,
						// 消失状态
						die: false,
						// 卡牌状态 1默认 2 翻转
						status: 1,
						// 卡牌对应值
						value: 1,
						// 卡牌翻转画面
						bg: '../../../static/supperzzle/1.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 1,
						bg: '../../../static/supperzzle/1.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 2,
						bg: '../../../static/supperzzle/2.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 2,
						bg: '../../../static/supperzzle/2.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 3,
						bg: '../../../static/supperzzle/3.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 3,
						bg: '../../../static/supperzzle/3.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 4,
						bg: '../../../static/supperzzle/4.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 4,
						bg: '../../../static/supperzzle/4.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 5,
						bg: '../../../static/supperzzle/5.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 5,
						bg: '../../../static/supperzzle/5.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 6,
						bg: '../../../static/supperzzle/6.png'
					},
					{
						roll: false,
						restore: false,
						die: false,
						status: 1,
						value: 6,
						bg: '../../../static/supperzzle/6.png'
					}
				]
			}
		}
	}
</script>

<style lang="scss">
	
	.content {
		width: 100%;
		height: 100vh;
		overflow: hidden;
		position: relative;
		background-image: url(@/static/supperzzle/bg.png);
		background-size: 100% 100%;
		
		background-repeat: no-repeat;
		
		.start{
			width: 100%;
			height: 100vh;
			overflow: hidden;
			background-color: rgba(0, 0, 0, 0.3);
			
			.icon{
				width: 60%;
				margin: 5vh 20%;
			}
			
			.max{
				animation: start 3s linear;
			}
			
			.btn{
				margin: 5vh 15vw;
				width: 70%;
				height: 80rpx;
				line-height: 80rpx;
				background-color: #ffaa00;
				border-radius: 40rpx;
				text-align: center;
				color: #fff;
			}
		}

		.title,.down {
			font-size: 60rpx;
			font-weight: bold;
			margin: 10vh auto;
			text-align: center;
			color: yellow;
			opacity: 0.8;
		}
		

		.box {
			width: 650rpx;
			height: 650rpx;
			background-color: rgba(255, 255, 255, 0.3);
			margin: 5vh auto;

			display: grid;
			grid-template-columns: 25% 25% 25% 25%;
			grid-template-rows: 33.3% 33.3% 33.3%;

			.item1 {
				width: 100%;
				height: 100%;
				background-image: url(@/static/supperzzle/card.png);
				background-size: 100% 100%;
			}


			.item2 {
				width: 100%;
				height: 100%;
			}

			.item3 {
				width: 100%;
				height: 100%;
			}

			.roll {
				animation: roll 1s forwards;
			}

			.die {
				animation: die 2s forwards;
			}



		}
		
		
		.end{
			width: 100%;
			height: 100vh;
			overflow: hidden;
			text-align: center;
			.success,.error{
				width: 80%;
				margin: 15vh 10%;
				
			}
			
			.again{
				margin: 5vh 25%;
				width: 50%;
				height: 70rpx;
				line-height: 70rpx;
				background: rgba(0, 0, 0, 0.5);
				border-radius: 40rpx;
				color: #ccc;
			}
			.again1{
				margin: 5vh 25%;
				width: 50%;
				height: 70rpx;
				line-height: 70rpx;
				background: #ffaa00;
				border-radius: 40rpx;
				color: #fff;
			}
		}



		@keyframes roll {
			from {
				-webkit-transform: translateZ(1000px) rotateY(0deg);
				transform: translateZ(1000px) rotateY(0deg);
			}

			to {
				-webkit-transform: translateZ(1000px) rotateY(360deg);
				transform: translateZ(1000px) rotateY(360deg);
			}
		}

		@keyframes die {
			from {
				transform: scale(1);
			}

			to {
				transform: scale(0);
			}
		}
		
		@keyframes start {
			0%{
				-webkit-transform: translateZ(1000px) rotateY(0deg) scale(1);
				transform: translateZ(1000px) rotateY(0deg) scale(1);
			}
			50%{
				-webkit-transform: translateZ(1000px) rotateY(360deg) scale(1);
				transform: translateZ(1000px) rotateY(360deg) scale(1);
			}
			75%{
				-webkit-transform: translateZ(1000px) rotateY(360deg) scale(0.5);
				transform: translateZ(1000px) rotateY(360deg) scale(0.5);
			}
			100%{
				-webkit-transform: translateZ(1000px) rotateY(360deg) scale(0);
				transform: translateZ(1000px) rotateY(360deg) scale(0);
			}
		}

	}
</style>