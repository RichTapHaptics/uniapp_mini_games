<template>
	<view class="content">

		<view v-if="!start">
			<image @tap="checkCar(1)" :class="check1" class="carBlue" src="@/static/car/CarBlue.png" mode="widthFix">
			</image>
			<image @tap="checkCar(2)" :class="check2" class="carRed" src="@/static/car/CarRed.png" mode="widthFix">
			</image>
			<view class="start" @tap="startGame()">
				开始游戏
			</view>

		</view>


		<view v-else>

			<view class="end" v-if="downTime == 0">
				<view class="title">
					游戏结束
				</view>
				<view class="info">
					本次一共行驶
					{{metre}}m
				</view>
				<view class="btn">
					<view class="left" @tap="back">
						返回
					</view>
					<view class="right" @tap="reset">
						再玩一次
					</view>
				</view>
			</view>
			<canvas class="game" canvas-id="myCanvas" @touchmove="onTouchMove" @touchstart="onTouchstart"
				:style="{width:canvasW+'px',height:canvasH+'px'}"></canvas>
		</view>

	</view>
</template>

<script>
	import { playExtPrebaked, initialize, quit, playHaptic } from '@/uni_modules/richtap-haptic-lite'
	import he1 from '@/static/car/speed.json'
	export default {
		data() {
			return {
				// 车辆选择
				check1: 'check',
				check2: '',
				start: false,
				player: null,
				// 游戏倒计时
				downTime: 60,
				downTimer: null,
				// 安全距离
				safeAreaHeight :0,
				// 屏幕宽度
				windowWidth: 0,
				// 屏幕高度
				windowHeight: 0,
				// 画布宽度
				canvasW: 0,
				// 画布高度
				canvasH: 0,
				// canvas 动画刷新器
				requestID: null,
				// 汽车图片对象
				car: {},
				// 背景图对象
				bg: {},
				// 障碍物图片对象
				hinder: {},
				// 油桶图对象
				oil: {},
				// 背景图数据
				bgData: {},
				// box数组
				boxs: [],
				// 马路数据
				roadData: {},
				// 汽车数据
				carData: {},
				// 成绩
				metre: 0,
				// canvas对象
				ctx: {},
				// 手指触摸x坐标
				touchStartX: null,
				// 手指触摸移动x坐标
				touchMoveX: null,
				speedHe: ''
			};
		},
		mounted() {
			console.log('car mounted');
			initialize({
				fail: (err) => {
					console.log(err.errCode);
				}
			})
			this.speedHe = JSON.stringify(he1)
		},
		onUnload() {
			console.log('car onUnload');
			quit({
				fail: (err) => {
					console.log(err.errCode);
				}
			})
		},
		onUnload() {
			if (this.player) {
				this.player.pause();
			}
			clearInterval(this.requestID);
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
			// 异步获取设备信息
			getSystemInfo() {
				let that = this;
				uni.getSystemInfo({
					success: function(res) {
						that.safeAreaHeight = res.safeArea.top;
						that.windowWidth = res.windowWidth;
						that.windowHeight = res.windowHeight;
						that.canvasW = res.windowWidth;
						that.canvasH = res.windowHeight;
						that.dev = res;
					}
				})
			},

			// 异步读取图片信息
			getImageInfo(image) {
				return new Promise((req, rej) => {
					uni.getImageInfo({
						src: image,
						success: function(res) {
							req(res)
						},
					});
				})
			},
			// 准备游戏数据
			async createGame() {
				// 获取canvas对象
				this.ctx = uni.createCanvasContext('myCanvas', this);
				// 获取设备信息
				this.getSystemInfo();

				// 加载背景图片
				this.bg = await this.getImageInfo('../../../static/car/bg.png');
				// 加载汽车图片
				let carPath = '';
				this.check1 ? carPath = '../../../static/car/CarBlue.png' : carPath = '../../../static/car/CarRed.png'
				this.car = await this.getImageInfo(carPath);

				// 加载障碍物图片
				this.hinder = await this.getImageInfo('../../../static/car/hinder.png');
				// 加载油桶图片
				this.oil = await this.getImageInfo('../../../static/car/oil.png');


				// 马路
				this.roadData = {
					width: this.canvasW * 0.8,
					height: this.canvasH,
					speed: 5,
					bg: "#47568e"
				}

				// 汽车
				this.carData = {
					path: this.car.path,
					width: this.car.width * 0.5,
					height: this.car.height * 0.5,
					x: (this.canvasW - this.car.width * 0.5) / 4,
					y: this.canvasH * 0.8
				}

				// 背景
				this.bgData = {
					path: this.bg.path,
					width: this.canvasW,
					height: this.canvasH,
					x: 0,
					y: 0,
					speed: 10,
					yPos: 0
				}
			},

			// 创建障碍物|油桶数组
			createBox() {
				let x, path, width, height, type;

				if (Math.random() < 0.1) {
					// 油桶出现的概率
					type = 1;
					path = this.oil.path;
					width = this.oil.width * 0.2;
					height = this.oil.height * 0.2;
					x = Math.random() * this.canvasW * 0.8 + this.canvasW * 0.1 - this.oil.width * 0.2;
				} else {
					// 设置障碍物
					type = 2;
					path = this.hinder.path;
					width = this.hinder.width * 0.2;
					height = this.hinder.height * 0.2;
					x = Math.random() * this.canvasW * 0.8 + this.canvasW * 0.1 - this.hinder.width * 0.2;
				}


				if (x < this.canvasW * 0.1 || x > this.canvasW * 0.9) {
					x = this.canvasW * Math.random() * this.canvasW * 0.5;
				}

				let box = {
					path: path,
					width: width,
					height: height,
					x: x,
					y: 10,
					speed: 10,
					type: type
				}
				this.boxs.push(box);

			},
			// 选择车辆
			checkCar(n) {
				this.playHaptic()
				this.check1 = '';
				this.check2 = '';
				n == 1 ? this.check1 = 'check' : this.check2 = 'check';
			},

			// 开始游戏
			async startGame() {
				this.playHaptic()
				this.start = true;
				this.player = uni.createInnerAudioContext();
				this.playMusic(0);
				await this.createGame();
				this.down();
				// 创建动画请求，不断重绘画布,16.7毫秒重绘一次，模仿约每秒60帧率
				// h5推荐使用requestAnimationFrame函数,setInterval性能不如requestAnimationFrame，但app不支持dom相关api
				let that = this;
				this.requestID = setInterval(() => {
					that.updateGame();
				}, 16.6);
			},

			// canvas 触摸事件
			onTouchstart(event) {

				this.touchStartX = event.changedTouches[0].x;
			},
			// canvas 触摸移动事件
			onTouchMove(event) {
				event.preventDefault();
				this.touchMoveX = event.changedTouches[0].x;
			},

			// 游戏倒计时
			down() {
				let that = this;
				this.downTimer = setInterval(function() {
					if (that.downTime == 0) {
						clearInterval(this.downTimer);
					} else {
						that.downTime--;
					}
				}, 1000)
			},

			// 音频播放
			playMusic(index) {
				let music = [{
						src: '../../../static/car/bgm.mp3',
						loop: true
					},
					{
						src: '../../../static/car/add.mp3',
						loop: false
					}
				];

				if (index == 1) {
					this.player.stop();
				}

				this.player.loop = music[index].loop;
				this.player.src = music[index].src;
				this.player.play();

				this.player.onPlay(() => {
					music[index].playing = true;
					console.log('开始播放' + index);
				});

				this.player.onError((res) => {
					console.log(res.errMsg);
					console.log(res.errCode);
				});;

				if (index == 1) {
					this.player.onEnded(() => {
						this.playMusic(0);
					})
				}

			},

			// 更新canvas
			updateGame() {
				let ctx = this.ctx;
				let that = this;
				// 清除画布
				ctx.clearRect(0, 0, this.canvasW, this.canvasH);

				// 绘制背景
				ctx.drawImage(this.bg.path, 0, this.bgData.yPos, this.bgData.width, this.bgData.height);
				ctx.drawImage(this.bg.path, 0, this.bgData.yPos - this.bgData.height, this.bgData.width, this.bgData
					.height);
				this.bgData.yPos += this.bgData.speed;
				if (this.bgData.yPos >= this.bgData.height) {
					this.bgData.yPos = 0;
				}

				// 绘制马路
				ctx.setFillStyle(this.roadData.bg);
				ctx.fillRect(this.canvasW * 0.1, 0, this.roadData.width, this.roadData.height);

				// 绘制马路虚线
				ctx.beginPath();
				ctx.moveTo(this.canvasW / 2, 0);
				ctx.lineTo(this.canvasW / 2, this.canvasH);
				ctx.closePath();
				ctx.lineWidth = 5;
				ctx.strokeStyle = "#ffffff";
				ctx.setLineDash([50]);
				ctx.stroke();

				// 移动赛车
				if (this.touchMoveX !== null) {
					// 根据触摸移动的距离来调整赛车的位置
					this.carData.x = this.touchMoveX;

					// 确保赛车在Canvas范围内
					if (this.carData.x < this.canvasW * 0.1) {
						this.carData.x = this.canvasW * 0.1;
					} else if (this.carData.x + this.carData.width > this.canvasW * 0.9) {
						this.carData.x = this.canvasW * 0.9 - this.carData.width;
					}

				}

				// 绘制汽车
				ctx.drawImage(this.car.path, this.carData.x, this.carData.y, this.carData.width, this.carData.height);
				// 绘制成绩
				this.metre = this.metre + 0.1;
				this.metre = parseFloat(this.metre.toFixed(2));
				ctx.setFontSize(15);
				ctx.shadowOffsetX = 7;
				ctx.shadowOffsetY = 7;
				ctx.shadowBlur = 5;
				ctx.shadowColor = "#000000";
				ctx.fillStyle = "#ffffff";
				ctx.fillText('当前成绩：' + this.metre + 'm', (this.canvasW / 2 - 105)/2, this.safeAreaHeight+20);
				// 绘制倒计时
				ctx.setFontSize(15);
				ctx.shadowOffsetX = 7;
				ctx.shadowOffsetY = 7;
				ctx.shadowBlur = 5;
				ctx.shadowColor = "#000000";
				ctx.fillStyle = "#ffffff";
				ctx.fillText('燃料续航：' + this.downTime + 's', this.canvasW/2+15, this.safeAreaHeight+20);

				// 绘制障碍物
				for (var i = 0; i < this.boxs.length; i++) {
					var box = this.boxs[i];
					box.y += box.speed;
					if (box.type == 1) {
						ctx.drawImage(this.oil.path, box.x, box.y, box.width, box.height);
					} else {
						ctx.drawImage(this.hinder.path, box.x, box.y, box.width, box.height);
					}

					// 检测赛车与障碍物碰撞
					if (
						this.carData.x < box.x + box.width &&
						this.carData.x + this.carData.width > box.x &&
						this.carData.y < box.y + box.height &&
						this.carData.y + this.carData.height > box.y
					) {

						if (box.type == 1) {
							// 碰撞油桶,增加游戏时间
							// this.playHaptic('RT_AWARD')
							playHaptic({
								heStr: this.speedHe,
								loop: 0,
								intensity: 255,
								interval: 0,
								frequency: 0,
							})
							this.playMusic(1);
							this.downTime = this.downTime + 3;
							this.boxs.splice(i, 1);
							ctx.setFontSize(15);
							ctx.shadowColor = "#ff0000";
							ctx.fillText('+3s', box.x, box.y);
							ctx.clearRect(box.x, box.y, box.width, box.height);
						} else {
							this.playHaptic('RT_BOMB')
							// 碰撞障碍物，游戏结束
							this.player.stop();
							this.downTime = 0;
							// 清除倒计时
							clearInterval(this.downTimer);
							// 取消动画请求
							clearInterval(this.requestID);
							return false;
						}


					}
				}

				// 障碍物和油桶出现的总概率
				if (Math.random() < 0.02) {
					this.createBox();
				}

				// 倒计时结束取消动画请求
				if (this.downTime == 0) {
					setTimeout(function() {
						that.player.stop();
						that.player.destroy();
						clearInterval(that.requestID);
						return false;
					}, 200)
				}

				// 写入画布
				ctx.draw();
			},

			back() {
				uni.navigateBack(1)
			},

			// 重置游戏状态
			reset() {
				this.carData.x = (this.canvasW - this.carData.width * 0.5) / 4,
					this.carData.y = this.canvasH * 0.8
				this.boxs = [];
				this.downTime = 60;
				this.metre = 0;
				this.startGame()
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
		background-image: url(@/static/car/bg.png);
		background-size: 100%;

		.game {
			margin: 0 auto;
		}

		.carBlue {
			width: 20%;
			position: absolute;
			left: 25%;
			top: 40%;
			border-radius: 30rpx;
		}

		.carRed {
			width: 20%;
			position: absolute;
			left: 55%;
			top: 50%;
			border-radius: 30rpx;
		}

		.check {
			box-shadow: 0 0 10px 5px rgba(255, 255, 127, 0.5);
		}

		.start {
			width: 30%;
			height: 80rpx;
			background-color: yellow;
			text-align: center;
			line-height: 80rpx;
			border-radius: 40rpx;
			position: absolute;
			left: 35%;
			bottom: 15%;
			color: #ff557f;
			text-shadow: 1px 1px 1px #000000;
		}

		.end {
			width: 70%;
			height: 350rpx;
			padding: 2%;
			background-color: #3e3e3e;
			position: absolute;
			left: 13%;
			top: 30%;
			z-index: 999999;
			text-align: center;

			.title {
				color: yellow;
				font-size: 45rpx;
				font-weight: bold;
			}

			.info {
				margin-top: 80rpx;
				color: #ff557f;
			}

			.btn {
				margin-top: 80rpx;
				display: flex;
				justify-content: space-between;
				align-items: center;
				font-weight: bold;

				.left {
					width: 40%;
					height: 70rpx;
					line-height: 70rpx;
					background-color: rgba(0, 0, 0, 0.2);
					border-radius: 40rpx;
					color: #ccc;
				}

				.right {
					width: 40%;
					height: 70rpx;
					line-height: 70rpx;
					background-color: yellow;
					border-radius: 40rpx;
				}
			}
		}

	}
</style>