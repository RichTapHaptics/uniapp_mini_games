<template>
	<view class="content">
		<image class="banner" src="@/static/index/banner.png" mode="widthFix"></image>

		<view class="list">
			<view class="game-item" v-for="item in gameBox" @tap="goInfo(item.path)">
				<image class="face" :src="item.faceImg" mode="widthFix"></image>
				<view class="name">{{item.name}}</view>
			</view>
		</view>

	</view>
</template>

<script>
	export default {
		data() {
			return {
				gameBox: [{
						name: '红包雨',
						path: '/pages/game/redBoxRain/index',
						faceImg: '../../static/index/redbox.jpg'
					},
					{
						name: '跳一跳',
						path: '/pages/game/jump/index',
						faceImg: '../../static/index/jump.png'
					},
					{
						name: '卡牌对对碰',
						path: '/pages/game/supperzzle/supperzzle',
						faceImg: '../../static/supperzzle/bg.png'
					},
					{
						name: '激情飞车',
						path: '/pages/game/car/index',
						faceImg: '../../static/index/car.jpg'
					},
					{
						name: '九宫格抽奖',
						path: '/pages/game/luckyGrid/luckyGrid',
						faceImg: '../../static/luckyGrid/bg.png'
					},
					{
						name: '敬请期待...',
						path: '',
						faceImg: '../../static/index/more.png'
					}
				]
			}
		},
		onLoad() {

		},
		methods: {
			goInfo(path) {
				if (path) {
					uni.navigateTo({
						url: path
					})
				} else {
					uni.showToast({
						icon: 'none',
						duration: 2000,
						title: '敬请期待...'
					})
				}
			}
		}
	}
</script>

<style lang="scss">


	.content {
		width: 100vw;
		height: 100vh;
		position: relative;

		.banner {
			width: 100%;
		}

		.list {
			width: 94%;
			padding: 3%;
			display: flex;
			justify-content: space-between;
			flex-wrap: wrap;

			.game-item {
				width: 48%;
				text-align: center;

				.face {
					width: 100%;
				}

				.name {
					position: relative;
					left: 0;
					bottom: 55rpx;
					height: 50rpx;
					line-height: 50rpx;
					background-color: #fff;
					opacity: 0.6;
					white-space: nowrap;
					overflow: hidden;
					text-overflow: ellipsis;
					color: #ff5500;
				}
			}
		}
	}
</style>