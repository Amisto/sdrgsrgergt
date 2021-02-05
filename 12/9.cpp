// TODO: fix the bug with wrong window sizeaaaaaa

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <random>
#include <chrono>
#include <cmath>

using std::cout;
using std::endl;
using std::list;

const double WX = 2560;
const double WY = 1361;
const double V_HERO = 200;
double V_ENEMY = 1;
const double V_ENEMY_ACCELERATION = 1;
const double V_BULLET = 1000;
const double D_HERO = 20;
const double D_ENEMY = 70;
const double D_BULLET = 10;
const double R_HERO = D_HERO / 2;
const double R_ENEMY = D_ENEMY / 2;
const double R_BULLET = D_BULLET / 2;
double CHANCE_TO_SPAWN_ENEMY = 0.0001;
const double PI = 3.14159265;

double get_time()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()/1e6;
}

int rand_uns(int min, int max)
{
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        static std::default_random_engine e(seed);
        std::uniform_int_distribution<int> d(min, max);
        return d(e);
}

sf::CircleShape spawn_enemy(double wx, double wy);
bool check(const sf::CircleShape &b, const sf::CircleShape &e);

int main()
{
    sf::Texture texture;
    sf::Texture bg_tx;
    if (!texture.loadFromFile("2.png"))
    {
        return 0;
    }
    if (!bg_tx.loadFromFile("s1200.jfif"))
    {
        return 0;
    }
    sf::RenderWindow window(sf::VideoMode(WX, WY), "My window", sf::Style::Fullscreen);

    double posx = WX / 2, posy = WY / 2, vel = 0.01;
    double scaleX = 1.0, scaleY = 1.0;

    sf::Sprite bg;
    bg.setTexture(bg_tx);
    bg.setPosition(sf::Vector2f(0, 0));
    auto bgs = bg_tx.getSize();
    bg.setScale(sf::Vector2f(WX/bgs.x, WY/(bgs.y - 50)));

    sf::Sprite hero;
    hero.setTexture(texture);
    hero.setTextureRect(sf::IntRect(0, 0, 115, 139));
    hero.setOrigin(sf::Vector2f(50, 50));
    hero.setPosition(sf::Vector2f(posx, posy));

    //hero.setFillColor(sf::Color(0, 255, 0));
    sf::Clock clock;
    sf::Time prev = clock.getElapsedTime();

    list<sf::CircleShape> enemies;
    list<std::pair<sf::CircleShape, sf::Vector2f>> bullets;

    //int spawn_bullet = 0;
    bool isPressed = false;

    while (window.isOpen())
    {
        sf::Time cur = clock.getElapsedTime();
        double dt = (cur - prev).asMicroseconds() * 1e-6;
        prev = cur;

        double wx = window.getSize().x, wy = window.getSize().y;
        scaleX = wx / WX;
        scaleY = wy / WY;

        hero.setScale(sf::Vector2f(1/scaleX, 1/scaleY));
        for (auto &e : enemies)
            e.setScale(sf::Vector2f(1/scaleX, 1/scaleY));
        for (auto &b : bullets)
            b.first.setScale(sf::Vector2f(1/scaleX, 1/scaleY));

        sf::Vector2f vel(V_HERO/scaleX, V_HERO/scaleY);

//==========================================================================

        if (rand_uns(0, 100000) / 100000.0 < CHANCE_TO_SPAWN_ENEMY)
            enemies.push_front(spawn_enemy(wx, wy));

//==========================================================================

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

//==========================================================================

        sf::Vector2f dir(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            dir.x = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            dir.x = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            dir.y = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            dir.y = 1;
        if (dir.x != 0 && dir.y != 0)
        {
            dir.x /= 1.41421356;
            dir.y /= 1.41421356;
        }
        posx += V_HERO * dir.x * dt / scaleX;
        posy += V_HERO * dir.y * dt / scaleY;

        if (posx < 0) posx = 0;
        if (posy < 0) posy = 0;
        if (posx > wx - D_HERO) posx = wx - D_HERO;
        if (posy > wy - D_HERO) posy = wy - D_HERO;

        hero.setPosition(sf::Vector2f(posx, posy));

//==========================================================================

//        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
//        {
//            double PI2 = 2 * PI, PISTEP = 2 * PI / 100;
//            for (double a = 0; a < PI2; a += PISTEP)
//            {
//                sf::CircleShape new_bullet(D_BULLET);
//                new_bullet.setPosition(posx + D_HERO / 2, posy + D_HERO / 2); // !!!
//                sf::Vector2f new_dir(sin(a), cos(a));
//                new_bullet.setFillColor(sf::Color(255, 0, 255));
//                bullets.push_front(std::pair<sf::CircleShape, sf::Vector2f>(new_bullet, new_dir));
//
//            }
//        }

        //spawn_bullet++;
        //if (spawn_bullet >= 500)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!isPressed)
            {
                sf::CircleShape new_bullet(D_BULLET);
                new_bullet.setPosition(posx + D_HERO / 2, posy + D_HERO / 2); // !!!
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                sf::Vector2f new_dir(localPosition.x - posx, localPosition.y - posy);
                double dm = sqrt(new_dir.x * new_dir.x + new_dir.y * new_dir.y);
                new_dir.x /= dm;
                new_dir.y /= dm;
                new_bullet.setFillColor(sf::Color(255, 0, 255));
                bullets.push_front(std::pair<sf::CircleShape, sf::Vector2f>(new_bullet, new_dir));
                isPressed = true;
            }
        }
        else
            isPressed = false;
        //}

//==========================================================================

        for (auto &e : enemies)
        {
            auto pos = e.getPosition();
            sf::Vector2f dir(posx - pos.x, posy - pos.y);
            double r = sqrt((posx - pos.x)*(posx - pos.x) + (posy - pos.y)*(posy - pos.y));
            dir.x /= V_ENEMY_ACCELERATION;
            dir.y /= V_ENEMY_ACCELERATION;
            dir.x += V_ENEMY * (posx - pos.x) / r;
            dir.y += V_ENEMY * (posy - pos.y) / r;
            dir.x *= dt;
            dir.y *= dt;
            pos += dir;
            e.setPosition(pos);
        }

//==========================================================================

        for (auto &b : bullets)
        {
            auto pos = b.first.getPosition();
            pos.x += dt * V_BULLET * b.second.x;
            pos.y += dt * V_BULLET * b.second.y;
            b.first.setPosition(pos);
        }

//==========================================================================

        bullets.remove_if([wx, wy](auto b) {  sf::Vector2f pos = b.first.getPosition();
                                return pos.x < - D_BULLET || pos.y < - D_BULLET
                                || pos.x > wx || pos.y > wy; });

//==========================================================================

        bullets.remove_if([&enemies](auto b) {
                        for (auto it = enemies.begin(); it != enemies.end(); it++)
                            if (check(b.first, *it))
                            {
                                auto c = it->getFillColor();
                                if (c.r < 60)
                                {
                                    enemies.erase(it);
                                    if (V_ENEMY < V_HERO) V_ENEMY += 1;
                                    else CHANCE_TO_SPAWN_ENEMY *= 1.01;
                                }
                                else
                                {
                                    c.r -= 50;
                                    c.b += 50;
                                    it->setFillColor(c);
                                }
                                return true;
                            }
                        return false;
                        });

//==========================================================================

//        enemies.remove_if([&hero](auto e) {
//                            if (check(hero, e))
//                            {
//                                sf::Color c = hero.getFillColor();
//                                if (c.g < 10)
//                                    exit(0);
//                                c.g -= 10;
//                                c.r += 10;
//                                hero.setFillColor(c);
//                                return true;
//                            }
//                            return false;
//                        });

//==========================================================================

        window.clear(sf::Color::Black);

        window.draw(bg);
        for (auto e : enemies)
            window.draw(e);
        for (auto b : bullets)
            window.draw(b.first);
        window.draw(hero);
        window.display();

        //int size = std::count_if(enemies.begin(), enemies.end(), [](auto i){return true;});
        //std::cout << size << std::endl;
    }

    return 0;
}



sf::CircleShape spawn_enemy(double wx, double wy)
{
    sf::CircleShape ret(D_ENEMY / 2);
    sf::Vector2f res;
    auto dir = rand_uns(1, 4);
    switch (dir)
    {
    case 1:
        res.x = - D_ENEMY;
        res.y = rand_uns(0, wy);
        break;
    case 2:
        res.x = wx;
        res.y = rand_uns(0, wy);
        break;
    case 3:
        res.x = rand_uns(0, wx);
        res.y = - D_ENEMY;
        break;
    case 4:
        res.x = rand_uns(0, wx);
        res.y = wy;
        break;
    }
    ret.setPosition(res);
    ret.setFillColor(sf::Color(255, 0, 0));
    return ret;
}


bool check(const sf::CircleShape &b, const sf::CircleShape &e)
{
    sf::Vector2f b_pos = b.getPosition();
    sf::Vector2f e_pos = e.getPosition();
    if (b_pos.x + R_BULLET < e_pos.x - R_ENEMY)
        return false;
    if (b_pos.x - R_BULLET > e_pos.x + R_ENEMY)
        return false;
    if (b_pos.y + R_BULLET < e_pos.y - R_ENEMY)
        return false;
    if (b_pos.y - R_BULLET > e_pos.y + R_ENEMY)
        return false;
    return true;
}
