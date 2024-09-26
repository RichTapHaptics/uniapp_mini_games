<template>
	<view class="content">
		<view class="bg-box">
			<image @click="musicStop()" class="music" mode="widthFix" src="@/static/jump/music-on.png" v-if="musicOn">
			</image>

			<image @click="musicPlay()" class="music" mode="widthFix" src="@/static/jump/music-off.png" v-else>
			</image>


			<image class="fd" mode="widthFix" v-for="(item,index) in fd" :key="index"
				:style="{top:item.top,left:item.left}" :src="item.src">
			</image>

			<image class="rabit" :style="rabitStyle" mode="widthFix" src="@/static/jump/rabit.png"></image>

			<image v-if="gameStatus>1" class="dice" mode="widthFix" src="@/static/jump/6.png" @tap="jump()">
			</image>

			<view v-if="gameStatus>1" class="play-btn" @tap="jump()">
				掷骰子
			</view>

		</view>

		<!-- 投骰子 -->
		<view v-if="diceShow" class="dice_bg" v-cloak>
			<image class="dice_gif" src="@/static/jump/dice.gif" mode="widthFix"></image>
		</view>

		<view class="dice_bg1" :style="diceBg" v-else-if="diceShow1" v-cloak>
		</view>

		<!-- 爆炸 -->
		<view class="peng" v-if="pengShow">
			<image class="peng-img" src="@/static/jump/peng.png" mode="widthFix"></image>
		</view>

		<!-- 遮罩 -->
		<view class="mask" v-if="maskShow">
		</view>

		<view v-if="gameStatus == 1">
			<image class="icon" src="@/static/jump/6.png" mode="widthFix"></image>

			<view class="start" @tap="start">
				开始游戏
			</view>

			<view class="clear" @tap="clear">
				清除缓存
			</view>
		</view>




	</view>
</template>

<script>
	export default {

		data() {
			return {
				// 游戏状态
				gameStatus: 1,
				maskShow: true,
				//骰子图片
				diceSrc: '',
				//骰子动画展示
				diceShow: false,
				//骰子静止展示
				diceShow1: false,
				//音乐播放状态
				musicOn: false,
				//音频对象
				innerAudioContext: null,
				//是否可以点击骰子
				playStatus: true,
				diceBg: {
					'background': 'url(../../../static/jump/dice-bg.png)',
					'background-repeat': 'no-repeat',
					'background-size': '100%'
				},
				// 爆炸状态
				pengShow: false,
				//当前位置
				now: 0,
				// 主角样式
				rabitStyle: {},
				//奖励惩罚位置
				fd: [{
						top: '69%',
						left: '47%',
						src: '../../../static/jump/fd.png'
					},
					{
						top: '67%',
						left: '57%',
						src: '../../../static/jump/bomb.png'
					},
					{
						top: '61.5%',
						left: '74%',
						src: '../../../static/jump/back.png'
					},{
						top: '56%',
						left: '57%',
						src: '../../../static/jump/bomb.png'
					}, {
						top: '50%',
						left: '38%',
						src: '../../../static/jump/fd.png'
					},  {
						top: '42%',
						left: '47%',
						src: '../../../static/jump/back.png'
					}, {
						top: '36%',
						left: '29%',
						src: '../../../static/jump/bomb.png'
					}, {
						top: '23%',
						left: '57%',
						src: '../../../static/jump/fd.png'
					}
				],
				// 格子坐标
				xy: [{
						top: "66%",
						left: "6%"
					},
					{
						top: "64%",
						left: "17%"
					},
					{
						top: "61%",
						left: "26%"
					},
					{
						top: "64%",
						left: "35%"
					},
					{
						top: "66.5%",
						left: "44%"
					},
					{
						top: "63.5%",
						left: "53%"
					},
					{
						top: "61%",
						left: "62%"
					},
					{
						top: "58.5%",
						left: "71%"
					},
					{
						top: "55.5%",
						left: "62%"
					},
					{
						top: "53%",
						left: "53%"
					},
					{
						top: "49.5%",
						left: "44%"
					},
					{
						top: "47.5%",
						left: "35%"
					},
					{
						top: "44%",
						left: "44%"
					},
					{
						top: "41%",
						left: "53%"
					},
					{
						top: "39%",
						left: "44%"
					},
					{
						top: "36%",
						left: "35%"
					},
					{
						top: "33.5%",
						left: "26%"
					},
					{
						top: "30%",
						left: "35%"
					},
					{
						top: "27%",
						left: "44%"
					},
					{
						top: "20.5%",
						left: "54%"
					}
				],

			};
		},

		onLoad(option) {
			// 初始化主角位置
			let now = uni.getStorageSync('now');
			if(now){
				this.now = now;
			}
			this.rabitStyle = this.xy[this.now];
			
			// 初始化音效
			
			this.innerAudioContext = uni.createInnerAudioContext();
			this.innerAudioContext.loop = true;
			this.innerAudioContext.src = '../../../static/jump/bgm.mp3';
			
		},
		onUnload(){
			this.musicStop()
		},
		onHide() {
			this.musicStop()
		},

		methods: {

			start() {
				
				this.gameStatus = 2;
				this.maskShow = false;
				this.musicPlay();
			},

			clear() {
				uni.clearStorage('now');
				this.now = 0;
				this.rabitStyle = this.xy[this.now];
				uni.showToast({
					icon: 'success',
					title: '缓存清理成功'
				})
			},

			musicPlay() {
				this.musicOn = true;
				this.innerAudioContext.play();
			},
			musicStop() {
				this.musicOn = false;
				this.innerAudioContext.pause();
			},


			closeAll() {
				this.diceShow = false; //骰子动画展示
				this.diceShow1 = false; //骰子静止展示
			},


			jump() {
				if(this.playStatus){
					this.playStatus = false;
					var that = this;
					// 骰子点数
					var num = Math.ceil(Math.random() * 6);
					// 开启遮罩
					that.maskShow = true;
					// 骰子旋转
					that.diceShow = true;
					setTimeout(function() {
						that.diceShow = false;
						that.diceShow1 = true;
						that.diceBg = {
							'background': 'url(../../../static/jump/dice/' + num + '.png)',
							'background-repeat': 'no-repeat',
							'background-size': '100%'
						}
						console.log(that.diceBg)
					}, 1500)
					
					var max = 0;
					setTimeout(function() {
						that.maskShow = false;
						that.diceShow1 = false;
						var t = setInterval(function() {
							max++;
							if (max > num) {
								uni.setStorageSync('now',that.now);
								that.playStatus = true;
								that.check();
								clearInterval(t);
							} else {
								that.now = Number(that.now) + 1;
								console.log(that.now);
								if (that.now > 19) {
									that.now = 0;
								}
								that.rabitStyle = that.xy[that.now];
							}
					
						}, 600)
					
					}, 3500)
				}
				
			},
			
			check(){
				if(this.now == 4 || this.now == 11 || this.now == 19){
					uni.showToast({
						icon:'none',
						duration:2500,
						title:'恭喜您获得1个福袋礼包'
					})
				}else if(this.now == 5 || this.now == 9 || this.now == 16){
					uni.showToast({
						icon:'none',
						duration:2000,
						title:'卧槽,炸弹...'
					})
					this.pengShow = true;
					this.now = 0;
					this.rabitStyle = this.xy[this.now];
					uni.setStorageSync('now',this.now);
					let that = this;
					setTimeout(function(){
						that.pengShow = false
					},3000)
				}else if(this.now == 7 || this.now == 14){
					uni.showToast({
						icon:'none',
						duration:2000,
						title:'不好意思,后退1步'
					})
					this.now = this.now - 1;
					this.rabitStyle = this.xy[this.now];
					uni.setStorageSync('now',this.now);
				}
			}
		}
	};
</script>

<style lang="scss" scoped>
	[v-cloak] {
		display: none !important;
	}

	.content {
		width: 100%;
		height: 100vh;
		background-image: url(@/static/jump/bg.png);
		background-size: 100% 100%;
		background-repeat: no-repeat;
		position: relative;

		.mask {
			position: absolute;
			top: 0;
			left: 0;
			width: 100%;
			height: 100vh;
			background-color: #000;
			opacity: 0.5;
			z-index: 8;
		}

		.icon {
			position: absolute;
			top: 30%;
			left: 25%;
			width: 50%;
			z-index: 8;
		}

		.start {
			position: absolute;
			top: 62%;
			left: 25%;
			width: 50%;
			height: 6vh;
			line-height: 6vh;
			text-align: center;
			border-radius: 50rpx;
			background-color: #ffaa00;
			z-index: 9;
			color: #fff;
		}

		.clear {
			position: absolute;
			top: 70%;
			left: 25%;
			width: 50%;
			height: 6vh;
			line-height: 6vh;
			text-align: center;
			border-radius: 50rpx;
			background-color: rgba(0, 0, 0, 0.5);
			z-index: 9;
			color: #fff;
		}




		.bg-box {
			width: 100%;
			height: 100%;

			.music {
				width: 50rpx;
				position: absolute;
				top: 2%;
				right: 3%
			}

			.rabit {
				width: 100rpx;
				position: absolute;

			}

			.fd {
				width: 60rpx;
				position: absolute;
			}



			.dice {
				width: 180rpx;
				position: absolute;
				top: 80%;
				left: 38%;

			}

			.play-btn {
				width: 180rpx;
				height: 50rpx;
				line-height: 50rpx;
				text-align: center;
				background-color: #fdba5e;
				border-radius: 30rpx;
				position: absolute;
				top: 95%;
				left: 38%;
				color: #fff;
			}




			.draw {
				width: 180rpx;
				position: absolute;
				top: 90%;
				right: 10%;

			}


		}

		.dice_bg {
			z-index: 999999;
			top: 15%;
			left: 12%;
			position: fixed;
			width: 80%;
			height: 867rpx;
			// line-height: 1250rpx;
			padding-top: 52%;
			background: url('@/static/jump/dice-bg.png');
			background-repeat: no-repeat;
			background-size: 100%;
			text-align: center;

			.dice_gif {

				width: 38%;
			}

		}

		.dice_bg1 {
			z-index: 999999;
			top: 15%;
			left: 12%;
			position: fixed;
			width: 80%;
			height: 867rpx;
			line-height: 1250rpx;
			background-repeat: no-repeat;
			background-size: 100%;
			text-align: center;
		}

		.peng {
			position: absolute;
			top: 30%;
			left: 25%;
			width: 50%;

			.peng-img {
				width: 100%;
			}

			animation: peng  6s forwards;
		}

		@keyframes peng {
			0% {
				transform: scale(1);
				opacity: 1;
			}

			50% {
				transform: scale(2);
				opacity: 0;
			}

			100% {
				transform: scale(0);
				opacity: 0;
			}
		}


	}
</style>