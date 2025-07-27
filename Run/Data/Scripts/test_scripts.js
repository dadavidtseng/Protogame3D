// test_scripts.js - Protogame3D JavaScript 測試腳本

// 基本測試函數
function basicTests() {
    console.log("=== 基本 JavaScript 測試 ===");

    // 數學運算
    var result = 10 + 5 * 2;
    console.log("數學運算 10 + 5 * 2 = " + result);

    // 字串操作
    var greeting = "Hello " + "Protogame3D!";
    console.log("字串連接: " + greeting);

    // 陣列操作
    var numbers = [1, 2, 3, 4, 5];
    var sum = 0;
    for (var i = 0; i < numbers.length; i++) {
        sum += numbers[i];
    }
    console.log("陣列總和: " + sum);

    // 物件操作
    var player = {
        name: "測試玩家",
        level: 10,
        health: 100
    };
    console.log("玩家資訊: " + player.name + ", 等級: " + player.level);
}

// 遊戲物件測試
function gameObjectTests() {
    console.log("=== 遊戲物件測試 ===");

    // 取得玩家位置
    var playerPos = Game.getPlayerPos();
    console.log("目前玩家位置: x=" + playerPos.x + ", y=" + playerPos.y + ", z=" + playerPos.z);

    // 建立立方體
    Game.createCube(0, 0, 5);
    console.log("在 (0, 0, 5) 建立立方體");

    // 移動道具
    Game.moveProp(0, 3, 3, 1);
    console.log("移動道具 0 到位置 (3, 3, 1)");
}

// 複雜圖案測試
function patternTests() {
    console.log("=== 複雜圖案測試 ===");

    // 建立圓形圖案
    function createCirclePattern(radius, count) {
        console.log("建立圓形圖案，半徑: " + radius + ", 物件數量: " + count);

        for (var i = 0; i < count; i++) {
            var angle = (i / count) * 2 * Math.PI;
            var x = Math.cos(angle) * radius;
            var y = Math.sin(angle) * radius;
            var z = 0;

            Game.createCube(x, y, z);
        }
    }

    // 建立網格圖案
    function createGridPattern(size, spacing) {
        console.log("建立網格圖案，大小: " + size + "x" + size + ", 間距: " + spacing);

        for (var x = 0; x < size; x++) {
            for (var y = 0; y < size; y++) {
                Game.createCube(x * spacing, y * spacing, 0);
            }
        }
    }

    // 建立螺旋圖案
    function createSpiralPattern(turns, radius) {
        console.log("建立螺旋圖案，圈數: " + turns + ", 半徑: " + radius);

        var steps = turns * 20; // 每圈 20 個點

        for (var i = 0; i < steps; i++) {
            var t = i / steps;
            var angle = t * turns * 2 * Math.PI;
            var currentRadius = t * radius;

            var x = Math.cos(angle) * currentRadius;
            var y = Math.sin(angle) * currentRadius;
            var z = t * 5; // 高度隨螺旋上升

            Game.createCube(x, y, z);
        }
    }

    // 執行圖案測試
    createCirclePattern(5, 8);
    createGridPattern(3, 2);
    createSpiralPattern(2, 8);
}

// 動畫測試函數
function animationTests() {
    console.log("=== 動畫測試 ===");

    // 模擬時間相關動畫
    var time = Date.now() / 1000; // 轉換為秒

    // 擺動動畫
    function createSwingingCubes() {
        console.log("建立擺動立方體動畫");

        for (var i = 0; i < 5; i++) {
            var x = i * 2;
            var y = Math.sin(time + i * 0.5) * 3; // 擺動
            var z = 2;

            Game.createCube(x, y, z);
        }
    }

    // 波浪動畫
    function createWavePattern() {
        console.log("建立波浪圖案");

        for (var x = -10; x <= 10; x++) {
            for (var y = -10; y <= 10; y++) {
                var distance = Math.sqrt(x * x + y * y);
                var height = Math.sin(distance - time * 2) * 2 + 3;

                Game.createCube(x, y, height);
            }
        }
    }

    createSwingingCubes();
    // createWavePattern(); // 註解掉，因為會建立太多立方體
}

// 數學工具函數
function mathUtils() {
    console.log("=== 數學工具測試 ===");

    // 距離計算
    function distance3D(x1, y1, z1, x2, y2, z2) {
        var dx = x2 - x1;
        var dy = y2 - y1;
        var dz = z2 - z1;
        return Math.sqrt(dx * dx + dy * dy + dz * dz);
    }

    // 線性插值
    function lerp(a, b, t) {
        return a + (b - a) * t;
    }

    // 測試數學函數
    var dist = distance3D(0, 0, 0, 3, 4, 0);
    console.log("點 (0,0,0) 到 (3,4,0) 的距離: " + dist);

    var interpolated = lerp(10, 20, 0.5);
    console.log("10 和 20 之間 50% 插值: " + interpolated);

    // 隨機位置生成
    function generateRandomPositions(count, range) {
        console.log("生成 " + count + " 個隨機位置，範圍: ±" + range);

        for (var i = 0; i < count; i++) {
            var x = (Math.random() - 0.5) * 2 * range;
            var y = (Math.random() - 0.5) * 2 * range;
            var z = Math.random() * range;

            Game.createCube(x, y, z);
            console.log("隨機立方體 " + i + ": (" + x.toFixed(2) + ", " + y.toFixed(2) + ", " + z.toFixed(2) + ")");
        }
    }

    generateRandomPositions(5, 10);
}

// 遊戲邏輯測試
function gameLogicTests() {
    console.log("=== 遊戲邏輯測試 ===");

    // 簡單的遊戲狀態管理
    var gameState = {
        score: 0,
        level: 1,
        enemies: [],
        powerUps: []
    };

    // 增加分數
    function addScore(points) {
        gameState.score += points;
        console.log("得分增加 " + points + "，總分: " + gameState.score);

        // 檢查升級
        if (gameState.score >= gameState.level * 1000) {
            gameState.level++;
            console.log("升級到等級 " + gameState.level + "!");
        }
    }

    // 創建敵人
    function spawnEnemy(x, y, z) {
        var enemy = {
            id: gameState.enemies.length,
            x: x,
            y: y,
            z: z,
            health: 100
        };

        gameState.enemies.push(enemy);
        Game.createCube(x, y, z);
        console.log("生成敵人 " + enemy.id + " 在位置 (" + x + ", " + y + ", " + z + ")");

        return enemy;
    }

    // 移動所有敵人
    function moveEnemies() {
        console.log("移動所有敵人...");

        for (var i = 0; i < gameState.enemies.length; i++) {
            var enemy = gameState.enemies[i];
            enemy.x += (Math.random() - 0.5) * 2;
            enemy.y += (Math.random() - 0.5) * 2;

            Game.moveProp(i, enemy.x, enemy.y, enemy.z);
            console.log("敵人 " + enemy.id + " 移動到 (" + enemy.x.toFixed(2) + ", " + enemy.y.toFixed(2) + ", " + enemy.z + ")");
        }
    }

    // 執行遊戲邏輯測試
    addScore(150);
    addScore(200);
    addScore(300);
    addScore(450);

    spawnEnemy(5, 5, 1);
    spawnEnemy(-3, 7, 1);
    spawnEnemy(8, -2, 1);

    moveEnemies();
}

// 主測試函數
function runAllTests() {
    console.log("開始執行所有 JavaScript 測試...");
    console.log("=====================================");

    basicTests();
    console.log("");

    gameObjectTests();
    console.log("");

    mathUtils();
    console.log("");

    patternTests();
    console.log("");

    animationTests();
    console.log("");

    gameLogicTests();
    console.log("");

    console.log("=====================================");
    console.log("所有測試完成！");
}

// 執行測試
runAllTests();