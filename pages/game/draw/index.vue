<template>
	<view class="content">
	
		<view class="draw-box" :class="bdStyle">
			<view v-for="(item,index) in prizes" class="item" :class="item.active?'item-active':''">
				<image class="item-img" :src="item.img" mode="widthFix" @tap="start(index)"></image>
			</view>
		</view>
	</view>
</template>

<script>
	export default {

		data() {
			return {
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
				// 抽奖当前旋中(order的下标)
				activeIndex: 0,
				// 奖品数组
				prizes: [{
						name: '基础足浴',
						active: false,
						img: '../../../static/draw/prize.png'
					},
					{
						name: '肝肾保健',
						active: false,
						img: '../../../static/draw/prize.png'
					},
					{
						name: '养生帝王套',
						active: false,
						img: '../../../static/draw/prize.png'
					},
					{
						name: '桑拿一条龙',
						active: false,
						img: '../../../static/draw/prize.png'
					},
					{
						name: '立即抽奖',
						active: false,
						img: '../../../static/draw/btn.png'
					},
					{
						name: 'Ktv总统套',
						active: false,
						img: '../../../static/draw/prize.png'
					},
					{
						name: '夜总会至尊卡',
						active: false,
						img: '../../../static/draw/prize.png'
					},
					{
						name: '法拉利458',
						active: false,
						img: '../../../static/draw/prize.png'
					},
					{
						name: '布加迪威龙',
						active: false,
						img: '../../../static/draw/prize.png'
					}
				]
			};
		},
		methods: {
			// 开始抽奖
			start(index) {
				let that = this;
				// 计算礼品最大下标
				let count = this.prizes.length - 2;
				if (index == 4 && !that.startStatus) {
					// 复原选中状态
					that.prizes[that.order[that.activeIndex]].active = false;
					// 复原选中下标
					that.activeIndex = 0;
					// 复原旋转状态
					that.endStatus = false;
					// 开启抽奖状态,避免暴击
					that.startStatus = true;
					// 开启闪烁动画
					that.bdStyle = 'bdStyle';
					
					let timer = setInterval(function() {
					
						that.prizes[that.order[that.activeIndex]].active = true;
						setTimeout(function() {
							if (that.endStatus && (that.endNumber == that.activeIndex)) {
								that.bdStyle = '';
								that.startStatus = false;
								clearInterval(timer);
								uni.showToast({
									icon: 'none',
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
			end(prizeId) {
				this.endNumber = prizeId;
				this.endStatus = true;
			}
		}
	}
</script>

<style lang="scss">
	.content {
		width: 100%;
		height: 100vh;
		overflow: hidden;
		background-image: url(@/static/draw/bg.png);
		background-size: 100% 100%;
		background-repeat: no-repeat;

		text-align: center;


		.draw-box {
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