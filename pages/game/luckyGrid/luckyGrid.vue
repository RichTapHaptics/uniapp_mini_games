<template>
	<view class="content">
	
		<view class="luckyGrid-box" :class="bdStyle">
			<view v-for="(item,index) in prizes" class="item" :class="item.active?'item-active':''">
				<image class="item-img" :src="item.img" mode="widthFix" @tap="start(index)"></image>
			</view>
		</view>
	</view>
</template>

<script>
	import { playExtPrebaked, initialize, quit } from '@/uni_modules/richtap-haptic-lite'
	export default {

		data() {
			return {
				// 音效对象
				innerAudioContext:null,
				// 开始抽奖
				startStatus:false,
				// 中奖礼品下标
				endNumber: '',
				// 结束旋转动画
				endStatus: false,
				// 抽奖框外框动画class
				bdStyle: '',
				// 抽奖旋转顺序(prize的下标)
				order: [0, 1, 2, 5, 8, 7, 6, 3],
				// 抽奖当选中(order的下标)
				activeIndex: 0,
				// 奖品数组
				prizes: [{
						name: '基础足浴',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					},
					{
						name: '肝肾保健',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					},
					{
						name: '养生帝王套',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					},
					{
						name: '桑拿一条龙',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					},
					{
						name: '立即抽奖',
						active: false,
						img: '../../../static/luckyGrid/btn.png'
					},
					{
						name: 'Ktv总统套',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					},
					{
						name: '夜总会至尊卡',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					},
					{
						name: '法拉利458',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					},
					{
						name: '布加迪威龙',
						active: false,
						img: '../../../static/luckyGrid/prize.png'
					}
				]
			};
		},
		mounted() {
			console.log('luckyGrid mounted');
			initialize({
				fail: (err) => {
					console.log(err.errCode);
				}
			})
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
			// 开始抽奖
			start (index) {
				let that = this;
				// 计算礼品最大下标
				let count = this.prizes.length - 2;
				if (index == 4 && !that.startStatus) {
					// 播放音效
					this.music();
					// 复原选中状态
					that.prizes[that.order[that.activeIndex]].active = false;
					// 复原选中下标
					// that.activeIndex = 0;
					// 复原旋转状态
					that.endStatus = false;
					// 开启抽奖状态,避免暴击
					that.startStatus = true;
					// 开启闪烁动画
					that.bdStyle = 'bdStyle';
					
					let timer = setInterval(function() {
						that.playHaptic()
						that.prizes[that.order[that.activeIndex]].active = true;
						setTimeout(function() {
							if (that.endStatus && (that.endNumber == that.activeIndex)) {
								that.playHaptic()
								// 抽奖结束
								// 关闭音效
								that.innerAudioContext.pause()
								// 清除边框闪烁动画
								that.bdStyle = '';
								// 关闭抽奖状态
								that.startStatus = false;
								// 清除抽奖动画定时器
								clearInterval(timer);
								uni.showToast({
									icon: 'none',
									duration:2500,
									title: '恭喜您获得' + that.prizes[that.order[that.activeIndex]].name
								})
							} else {
								that.prizes[that.order[that.activeIndex]].active = false;
								that.activeIndex++;
								if (that.activeIndex > count) that.activeIndex = 0;
							}

						}, 250)

					}, 150)

					// 模拟请求后端,获取中奖礼品
					setTimeout(function() {
						that.end(Math.ceil(Math.random() * count))
					}, 5000)
				}
			},
			// 结束抽奖
			end (prizeId) {
				this.endNumber = prizeId;
				this.endStatus = true;
			},
			// 播放音效
			music () {
				this.innerAudioContext = uni.createInnerAudioContext();
				this.innerAudioContext.autoplay = true;
				this.innerAudioContext.loop = true;
				this.innerAudioContext.src = '../../../static/luckyGrid/bgm.mp3';
				this.innerAudioContext.onPlay(() => {
					console.log('开始播放');
				});
				this.innerAudioContext.onError((res) => {
					console.log(res.errMsg);
					console.log(res.errCode);
				});
			}
		}
	}
</script>

<style lang="scss">
	.content {
		width: 100%;
		height: 100vh;
		overflow: hidden;
		background-image: url(@/static/luckyGrid/bg.png);
		background-size: 100% 100%;
		background-repeat: no-repeat;

		text-align: center;


		.luckyGrid-box {
			width: 500rpx;
			height: 500rpx;
			padding: 20rpx;
			border: 10px solid #55007f;
			background-color: #DDD6F3;
			border-radius: 50rpx;
			margin: 40vh auto;
			display: flex;
			flex-wrap: wrap;
			justify-content: space-between;
			align-items: center;

			.item {
				width: 30%;
				height: 150rpx;
				background-color: #f1f1f1;
				border-radius: 20rpx;

				.item-img {
					width: 80%;
					margin: 10% auto;
				}
			}

			.item-active {
				background-color: #FAACA8;
			}



		}

		.bdStyle {
			animation: bd 0.6s infinite;
		}

		@keyframes bd {

			0% {
				border: 10px solid #FF3CAC;
			}
			
			50% {
				border: 10px solid #784BA0;
			}

			100% {
				border: 10px solid #2B86C5;
			}
		}


	}
</style>