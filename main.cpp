#include <iostream>
#include<vector>
#include <SDL.h>
#include <SDL_image.h>
#include<algorithm>
#include <cmath> // Thêm include này
#include <ctime>
using namespace std;
const int SCREEN_WIDTH=800;
const int SCREEN_HEIGHT=600;
const int TILE_SIZE=40;
const int MAP_WIDTH=SCREEN_WIDTH/TILE_SIZE;
const int MAP_HEIGHT=SCREEN_HEIGHT/TILE_SIZE;
SDL_Texture *playerTexture;
SDL_Texture *enemyTexture;
SDL_Texture *bulletTexture;
SDL_Texture *wallTexture;
class Bullet{
public:
    int x,y;
    int dx,dy;
    SDL_Rect rect;
    bool active;
    Bullet(int startX,int startY,int dirX,int dirY)
    {
        x=startX;
        y=startY;
        dx=dirX;
        dy=dirY;
        active=true;
        rect={x,y,10,10};
    }
    void move() {
    x += dx;
    y += dy;
    rect.x = x;
    rect.y = y;
    if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE ||
        y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE) {
        active = false;
    }
    }

    void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL_RenderFillRect(renderer, &rect);
        double angle = atan2(dy, dx) * 180 / M_PI; // Tính góc xoay

        SDL_Point center;
        center.x = rect.w / 2;
        center.y = rect.h / 2;

        SDL_RenderCopyEx(renderer, bulletTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }
    }
    };
class Wall{
public:
    int x,y;
    SDL_Rect rect;
    bool active;

    Wall(int startX, int startY)
    {
        x = startX;
        y = startY;
        active = true;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
    }
   void render(SDL_Renderer* renderer) {
    if (active) {
        //SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer,wallTexture,NULL,&rect);

    }
   }
};
class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    int directionChangeTimer=0;
    std::vector<Bullet> bullets;

    EnemyTank(int startX, int startY) {
        moveDelay = 15; // Delay for movement
        shootDelay = 5;  // Delay for shooting
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = 1;
        active = true;
    }
    void move(const std::vector<Wall>& walls) {
        if (--moveDelay > 0) return;
        moveDelay = 10;

        int newX = x + this->dirX * 5;
        int newY = y + this->dirY * 5;

        SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
        for (const auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
                return;
            }
        }

        if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
            newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
            x = newX;
            y = newY;
            rect.x = x;
            rect.y = y;
        }
    }
void shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                               this->dirX, this->dirY));
}

void updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}
 void changeDirection(const std::vector<Wall>& walls) {
        directionChangeTimer++;
        if (directionChangeTimer >= 240) {
            directionChangeTimer = 0;

            int r = rand() % 4;
            if (r == 0) {
                dirX = 0;
                dirY = -1;
            }
            else if (r == 1) {
                dirX = 0;
                dirY = 1;
            }
            else if (r == 2) {
                dirY = 0;
                dirX = -1;
            }
            else if (r == 3) {
                dirY = 0;
                dirX = 1;
            }

            int newX = x + dirX * 7;
            int newY = y + dirY * 7;
            SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
            for (const auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
                   changeDirection(walls);
                    return;
                }
            }
        }
    }
void render(SDL_Renderer* renderer) {
    double angle = atan2(dirY, dirX) * 180 / M_PI;

    SDL_Point center;
    center.x = rect.w / 2;
    center.y = rect.h / 2;

    SDL_RenderCopyEx(renderer, enemyTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);

    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

};
class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector<Bullet> bullets;
    PlayerTank (int startX, int startY) {
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = -1; // Default direction up
    }

    void move(int dx,int dy,const vector<Wall> &walls)
    {
        int newX=x+dx;
        int newY=y+dy;
        this->dirX=dx;
        this->dirY=dy;
        SDL_Rect newRect={newX,newY,TILE_SIZE,TILE_SIZE};
        for(std::vector<Wall>::size_type i=0;i<walls.size();i++) // Corrected loop counter type
            if(walls[i].active&&SDL_HasIntersection(&newRect,&walls[i].rect))  return;
        if(newX>=TILE_SIZE&&newY>=TILE_SIZE&&newX<=SCREEN_WIDTH-2*TILE_SIZE&&newY<=SCREEN_HEIGHT-2*TILE_SIZE)
        {
            x=newX;
            y=newY;
            rect.x=x;
            rect.y=y;
        }
    }
    void shoot() {
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                               this->dirX, this->dirY));
    }

    void updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
    }
    void render(SDL_Renderer* renderer)
    {
       double angle = atan2(dirY, dirX) * 180 / M_PI; // Tính góc xoay

        SDL_Point center;
        center.x = rect.w / 2;
        center.y = rect.h / 2;

        SDL_RenderCopyEx(renderer, playerTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
        for(auto &bullet :bullets) bullet.render(renderer);
    }
};
class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;
    vector <Wall> walls;
    //PlayerTank player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
    PlayerTank player=PlayerTank(0,0);
    int enemyNumber =3;
    vector<EnemyTank> enemies;
    void generateWalls() {
    for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
        }
    }
    }
    //funcion
    Game()
    {
        running=true;
        if(SDL_Init(SDL_INIT_VIDEO)<0)
        {
            cerr<<"Error"<<SDL_GetError()<<endl;
            running=false;
        }
        window=SDL_CreateWindow("Battle_City",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
        if(!window)
        {
            cerr<<"Window no creater"<<SDL_GetError<<endl;
            running =0;
        }
        renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
        if(!renderer)
        {
            cerr<<"Renderer no creater"<<SDL_GetError<<endl;
            running =0;
        }
         // Khởi tạo SDL_image sau khi renderer được tạo
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
            running = false;
            return; // Thoát nếu không khởi tạo được SDL_image
        }
        playerTexture =IMG_LoadTexture(renderer,"playerTank.png");
        if(!playerTexture){cerr<<"loi";}
        enemyTexture =IMG_LoadTexture(renderer,"enemyTank.png");
        if(!enemyTexture){cerr<<"loi";}
        bulletTexture =IMG_LoadTexture(renderer,"bullet.png");
        if(!bulletTexture){cerr<<"loi";}
        wallTexture =IMG_LoadTexture(renderer,"wall.png");
        if(!wallTexture){cerr<<"loi";}
        generateWalls();
        player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
        spawnEnemies();
    };
    void updateEnemyTanks() {
    for (auto& enemy : enemies) {
        // Tính khoảng cách giữa enemy và player
        int dx = player.x - enemy.x;
        int dy = player.y - enemy.y;

        // Nếu player nằm trên cùng 1 đường, enemy có thể bắn
        if ((dx == 0 || dy == 0) && (rand() % 100 < 5)) {
            enemy.shoot(); // Enemy bắn với tỉ lệ 5% mỗi frame nếu có tầm nhìn
        }

        // Nếu player ở gần, địch sẽ đuổi theo
        if (abs(dx) + abs(dy) < 200) {
            if (abs(dx) > abs(dy)) {
                enemy.dirX = (dx > 0) ? 1 : -1;
                enemy.dirY = 0;
            } else {
                enemy.dirY = (dy > 0) ? 1 : -1;
                enemy.dirX = 0;
            }
        } else {
            // Nếu không thấy player, di chuyển ngẫu nhiên nhưng tránh tường
            if (rand() % 100 < 10) { // 10% cơ hội thay đổi hướng
                int r = rand() % 4;
                enemy.dirX = (r == 0) ? -1 : (r == 1) ? 1 : 0;
                enemy.dirY = (r == 2) ? -1 : (r == 3) ? 1 : 0;
            }
        }

        // Kiểm tra va chạm với tường trước khi di chuyển
        int newX = enemy.x + enemy.dirX * 5;
        int newY = enemy.y + enemy.dirY * 5;
        SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };

        bool collision = false;
        for (const auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
                collision = true;
                break;
            }
        }

        if (!collision) {
            enemy.move(walls);
        } else {
            // Nếu bị kẹt, đổi hướng
            int r = rand() % 4;
            enemy.dirX = (r == 0) ? -1 : (r == 1) ? 1 : 0;
            enemy.dirY = (r == 2) ? -1 : (r == 3) ? 1 : 0;
        }
    }
}

    void Events()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_QUIT) running=0;
            else if(event.type==SDL_KEYDOWN)
            switch(event.key.keysym.sym)
            {
                case SDLK_UP:
                    player.dirX = 0;
                    player.dirY = -1;
                    player.move(0, -5, walls);
                    break;
            case SDLK_DOWN:
                player.dirX = 0;
                player.dirY = 1;
                player.move(0, 5, walls);
                break;
            case SDLK_LEFT:
                player.dirX = -1;
                player.dirY = 0;
                player.move(-5, 0, walls);
                break;
            case SDLK_RIGHT:
                player.dirX = 1;
                player.dirY = 0;
                player.move(5, 0, walls);
                break;
                case SDLK_SPACE:player.shoot();break;
            }
        }
    }
    void spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }
        enemies.push_back(EnemyTank(ex, ey));
    }
}
    void render()
    {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
    SDL_RenderClear(renderer); // delete color

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
        for (int j = 1; j < MAP_WIDTH - 1; ++j) {
            SDL_Rect tile = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    for(std::vector<Wall>::size_type i=0;i<walls.size();i++) walls[i].render(renderer);
    player.render(renderer);
    for(auto &enemy:enemies)
    {
        enemy.render(renderer);
    }
    SDL_RenderPresent(renderer);
    }
    void update() {
    player.updateBullets();
    updateEnemyTanks();
     for (auto& enemy : enemies) {
        enemy.move(walls);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot();
        }
    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
    }

    for (auto& bullet : player.bullets) {
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
            }
        }
    }
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                enemy.active = false;
                bullet.active = false;
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](EnemyTank &e) { return !e.active; }), enemies.end());

    if (enemies.empty()) {
        running = false;
    }
        for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            // Update
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                running = false;
                return;
            }
        }
    }

}
    void run()
    {
        while(running)
        {
            Events();
            update();
            render();
            SDL_Delay(16);
        }
    }
    ~Game()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    };

};
int main(int argc,char* argv[])
{
    Game game;
    if(game.running)
    {
        game.run();
    }
    return 0;
}


