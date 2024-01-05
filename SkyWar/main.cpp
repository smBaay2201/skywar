#include <sl.h>
#include <vector>
#include <Windows.h>
#include <random>
#include "entities.h"
#include <cmath>

using namespace std;

int main()
{
    random_device rd;
    slWindow(720, 1280, "SkyWar", 0);

    int imgBg = slLoadTexture("D:\\Amikom\\Pemograman Algoritma\\Tugas Proyek UAS\\assets\\galaxy.png");
    int imgPsw = slLoadTexture("D:\\Amikom\\Pemograman Algoritma\\Tugas Proyek UAS\\assets\\playership.png");
    int imgEnmy = slLoadTexture("D:\\Amikom\\Pemograman Algoritma\\Tugas Proyek UAS\\assets\\enemyship.png");
    int bgsound = slLoadWAV("D:\\Amikom\\Pemograman Algoritma\\Tugas Proyek UAS\\assets\\bg.wav");
    int imgpelur = slLoadTexture("D:\\Amikom\\Pemograman Algoritma\\Tugas Proyek UAS\\assets\\laser_player.png");

    int score = 0;

    const double gravity = -0.011;
    std::vector<enm> enemies;
    for (int i = 0; i < 3; i++)
    {
        double x = 360 + i * 1000;
        double y = (rd() % 3 + 1) * 150;
        enemies.push_back({ x, y, 30 });
    }

    bg background;
    background.posX = 360;
    background.posY = 640;

    psw player;
    player.posX = 360;
    player.posY = 150;
    player.velX = 0.0;
    player.velY = 0.0;
    player.health = 100;
    player.isShooting = false;

    bullet bulletPlayer;
    bulletPlayer.posX = player.posX;
    bulletPlayer.posY = player.posY;
    bulletPlayer.velY = 10.0; // Kecepatan vertikal peluru (ditingkatkan dari 5.0)
    bulletPlayer.active = false;

    const double playerHitboxSize = 50.0;
    const double enemyHitboxSize = 50.0;


    double scrollY = 0.0;

    slSoundLoop(bgsound);
    bool gameover = false;
    while (!slShouldClose())
    {
        slSetSpriteScroll(0.0, scrollY);
        slSprite(imgBg, background.posX, background.posY, 1080, 2272);
        scrollY += 0.001;

        slSetSpriteScroll(0.0, 0.0);

        // Perbarui posisi pesawat sesuai dengan posisi mouse
        player.posX = slGetMouseX();
        player.posY = slGetMouseY();

        if (slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
        {
            player.isShooting = true;
        }
        else
        {
            player.isShooting = false;
        }

        if (player.isShooting && !bulletPlayer.active)
        {
            bulletPlayer.active = true;
            bulletPlayer.posX = player.posX;
            bulletPlayer.posY = player.posY;
        }
        
        if (bulletPlayer.active)
        {
            bulletPlayer.posY += bulletPlayer.velY;

            // Periksa tabrakan dengan musuh
            for (enm& enemy : enemies)
            {

                if (enemy.posY <= bulletPlayer.posY && bulletPlayer.posY <= enemy.posY + 101 &&
                    enemy.posX <= bulletPlayer.posX && bulletPlayer.posX <= enemy.posX + 119)
                {
                    // Tabrakan terjadi, atur kembali posisi musuh dan nonaktifkan peluru
                    enemy.posY = 1000;
                    enemy.posX = (rd() % 3 + 1) * 150;
                    bulletPlayer.active = false;
                }

            }


            // Nonaktifkan peluru jika mencapai batas atas layar
            if (bulletPlayer.posY > 1000)
            {
                bulletPlayer.active = false;
            }
           
        }

        // Rendering musuh
        for (const enm& enemy : enemies)
        {
            slSprite(imgEnmy, enemy.posX, enemy.posY, 119, 101);
        }



        // Rendering pesawat pemain
        slSprite(imgPsw, player.posX, player.posY, 110, 110);

        // Rendering peluru
        if (bulletPlayer.active)
        {
            slSprite(imgpelur, bulletPlayer.posX, bulletPlayer.posY, 20, 60);
        }

        for (enm& enemy : enemies)
        {
            double angle = atan2(player.posY - enemy.posY, player.posX - enemy.posX);
            double speed = 2.0;  // Sesuaikan dengan kecepatan yang diinginkan
            enemy.velX = speed * cos(angle);
            enemy.velY = speed * sin(angle);

            enemy.posX += enemy.velX;
            enemy.posY += enemy.velY;
        }
        for (const enm& enemy : enemies)
        {
            slSprite(imgEnmy, enemy.posX, enemy.posY, 119, 101);
        }

        // Periksa tabrakan pesawat dengan musuh
        for (const enm& enemy : enemies)
        {
            double distance = sqrt(pow(player.posX - enemy.posX, 2) + pow(player.posY - enemy.posY, 2));
            if (distance < playerHitboxSize + enemyHitboxSize)
            {
                gameover = true;
                break;
            }
        }

        // Periksa tabrakan peluru dengan musuh
        if (bulletPlayer.active)
        {
            for (enm& enemy : enemies)
            {
                double distance = sqrt(pow(bulletPlayer.posX - enemy.posX, 2) + pow(bulletPlayer.posY - enemy.posY, 2));
                if (distance < 0.5 * (playerHitboxSize + enemyHitboxSize))
                {
                    // Tabrakan terjadi, atur kembali posisi musuh dan nonaktifkan peluru
                    enemy.posY = 1000;
                    enemy.posX = (rd() % 3 + 1) * 150;
                    bulletPlayer.active = false;
                }
            }
        }

        // Periksa jika gameover
        //if (gameover)
        //{
            // Tambahkan logika yang diperlukan untuk menangani gameover, misalnya menghentikan suara dan menampilkan pesan game over
            //slSoundStop(bgsound);
            //slText(360, 640, "Game Over");
            //slRender();

            // Tunggu sampai pengguna menekan tombol tertentu atau waktu tertentu tanpa menggunakan slSleep atau slDelay
            //while (!slGetKey(SL_KEY_ESCAPE) && !slShouldClose())
            //{
            //    slRender();
          //  }

        //}
        
        slRender();
    }

    return 0;
}
