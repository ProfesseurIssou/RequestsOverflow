#include <iostream>                                                                                                 //cout endl
#include <string>
#include <SFML/Graphics.hpp>

#include "config.h"
#include "ResourcesManager.h"				                                                                        //Gestion des resources
#include "MainMenu.h"                                                                                               //Menu principale
#include "Game.h"                                                                                                   //Game
#include "GameOver.h"                                                                                               //Gameover

typedef unsigned int uint;                                                                                          //Simplification

/*SETTINGS*/
struct gfxSettings{                                                                                                 //Parametre graphique
    uint screenWith = 1080;                                                                                             //Largeur en pixel de l'ecran
    uint screenHeight = 720;                                                                                            //Hauteur en pixel de l'ecran
    char screenName[21] = "RequestsOverflow";                                                                           //Titre de la fenetre
    uint FPSLimit = 60;                                                                                                 //Limitation du nombre de FPS
    bool mouseVisible = true;                                                                                           //Si on doit afficher le curseur
}gfx;
struct viewSettings{                                                                                                //Parametre de la vue
    int minX;
    int minY;
    int maxX;
    int maxY;
}viewSettings;
/*########*/


int main(){
    /*SETUP*/
    sf::RenderWindow gameWindow(sf::VideoMode(gfx.screenWith,gfx.screenHeight),gfx.screenName);                     //On cree la fenetre de jeux
    gameWindow.setFramerateLimit(gfx.FPSLimit);                                                                     //Limitation des FPS
    gameWindow.setKeyRepeatEnabled(REPEAT_KEY);                                                                     //Repetition des touche du clavier
    gameWindow.setMouseCursorVisible(gfx.mouseVisible);                                                             //Si on doit afficher le curseur
    sf::View gameView(sf::FloatRect(0,0,gfx.screenWith,gfx.screenHeight));                                          //Point de vue du jeux
    viewSettings.minX = 0;
    viewSettings.minY = 0;
    viewSettings.maxX = gfx.screenWith;
    viewSettings.maxY = gfx.screenHeight;
    /*#####*/

    /*SCENES*/
    ResourcesManager resourcesManager;                                                                              //Gestionnaire des resources
    MainMenu mainMenu(&gameWindow);                                                                                 //Creation du menu principale
    Game *game=NULL;                                                                                                //Creation de la variable du jeux
    GameOver gameover(&gameWindow);                                                                                 //Creation de la scene de gameover
    /*######*/

    /*STATES*/
    uint scene = SCENE_MAINMENU;                                                                                    //Scene actuel, par defaut le menu principale
    uint score = 0;                                                                                                 //Score a la fin du jeux
    bool play = true;                                                                                               //Si le jeux tourne
    bool up = false;                                                                                                //Touche Haut
    bool down = false;                                                                                              //Touche Bas
    bool left = false;                                                                                              //Touche Gauche
    bool right = false;                                                                                             //Touche Droite
    bool R_key = false;                                                                                             //Touche R
    bool mouseLeftClick = false;                                                                                    //Clique gauche
    bool mouseRightClick = false;                                                                                   //Clique droit
    int mouseScroll = 0;                                                                                            //Roulette sourie
    bool clickLock = false;                                                                                         //Si on doit prendre en compte le clique de sourie ou pas (pour les changement de scene afin de laisser le temp de retirer le click)
    /*######*/

    /*VARIABLES*/
    uint i;                                                                                                         //Compteur de boucle
    sf::Vector2i mousePosWindow;                                                                                    //Position de la sourie sur la fenetre
    sf::Vector2f mousePos;                                                                                          //Position de la sourie dans le monde
    /*#########*/

    /*GAMELOOP*/
    while(play){                                                                                                    //Tant que le jeux tourne
        /*EVENTS*/
        mouseScroll = 0;                                                                                                //On remet le scroll a 0
        bool mouseScrollCache = false;                                                                                  //Pour compté qu'une seule fois le scroll par Frame
        sf::Event event;                                                                                                //On cree l'objet evenement
        while(gameWindow.pollEvent(event)){                                                                             //Pour tout les evenements de l'ecran
            mousePosWindow = sf::Mouse::getPosition(gameWindow);                                                            //Recuperation de la position de la sourie sur la fenetre
            mousePos = gameWindow.mapPixelToCoords(mousePosWindow);                                                         //Convertion de la position
            if(event.type == sf::Event::Resized){                                                                           //Si la fenetre est redimentionné
                viewSettings.maxX = event.size.width;                                                                           //On redefinie la largeur de la vue
                viewSettings.maxY = event.size.height;                                                                          //On redefinie la hauteur de la vue
            }
            
            if(event.type == sf::Event::Closed)play = false;                                                                //Si on ferme la fenetre => Arret du jeux            
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)up = true;                             //Si on appuis sur la touche du haut => Touche du haut pressé
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)up = false;                           //Si on relache la touche du haut => Touche du haut relaché
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)down = true;                         //Si on appuis sur la touche du bas => Touche du bas pressé
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down)down = false;                       //Si on relache la touche du bas => Touche de bas relaché
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)left = true;                         //Si on appuis sur la touche de gauche => Touche du gauche pressé
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left)left = false;                       //Si on relache la touche du gauche => Touche de gauche relaché
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)right = true;                       //Si on appuis sur la touche du droite => Touche du droite pressé
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right)right = false;                     //Si on relache la touche du droite => Touche de droite relaché
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)R_key = true;                           //Si on appuis sur la touche R => Touche R pressé
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)R_key = false;                         //Si on relache la touche R => Touche R relaché

            if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)mouseLeftClick = true;          //Si on appuis sur le clique gauche => Clique gauche appuyé
            if(event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)mouseLeftClick = false;        //Si on relache le clique gauche => Clique gauche relaché
            if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Right)mouseRightClick = true;        //Si on appuis sur le clique droit => Clique droit appuyé
            if(event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Right)mouseRightClick = false;      //Si on relache le clique droit => Clique droit relaché
            if(event.type == sf::Event::MouseWheelMoved)mouseScroll = -event.mouseWheel.delta;                                  //Si la molette scroll => Scroll de la molette (inversé)
        }
        /*######*/

        /*LOGIC*/
        if(mouseLeftClick && clickLock){                                                                            //Si on clique ET que le clickLock est activé
            mouseLeftClick=false;                                                                                       //On annule le clique de sourie
        }else{                                                                                                      //Sinon
            clickLock = false;                                                                                          //On désactive le lock
        }
        switch(scene){                                                                                              //Pour chaque valeur de scene
            case SCENE_MAINMENU:                                                                                        //Si on est sur le menu principale
                gameView.reset(sf::FloatRect(0,0,viewSettings.maxX,viewSettings.maxY));                                     //On remet la vue a 0
                break;
            case SCENE_GAME_EASY:                                                                                       //Si on est sur le mode Facile
                if(up)gameView.move(0,-VIEW_SPEED);                                                                         //Si monte
                if(down)gameView.move(0,VIEW_SPEED);                                                                        //Si descend
                if(left)gameView.move(-VIEW_SPEED,0);                                                                       //Si gauche
                if(right)gameView.move(VIEW_SPEED,0);                                                                       //Si droite
                gameView.zoom(1+((float)mouseScroll/10));                                                                   //Application du zoom
                break;
            case SCENE_GAME_MEDIUM:                                                                                     //Si on est sur le mode moyen
                if(up)gameView.move(0,-VIEW_SPEED);                                                                         //Si monte
                if(down)gameView.move(0,VIEW_SPEED);                                                                        //Si descend
                if(left)gameView.move(-VIEW_SPEED,0);                                                                       //Si gauche
                if(right)gameView.move(VIEW_SPEED,0);                                                                       //Si droite
                gameView.zoom(1+((float)mouseScroll/10));                                                                   //Application du zoom
                break;
            case SCENE_GAME_MASTER:                                                                                     //Si on est sur le mode master
                if(up)gameView.move(0,-VIEW_SPEED);                                                                         //Si monte
                if(down)gameView.move(0,VIEW_SPEED);                                                                        //Si descend
                if(left)gameView.move(-VIEW_SPEED,0);                                                                       //Si gauche
                if(right)gameView.move(VIEW_SPEED,0);                                                                       //Si droite
                gameView.zoom(1+((float)mouseScroll/10));                                                                   //Application du zoom
                break;
            case SCENE_GAMEOVER:                                                                                        //Si on est sur le gameover
                gameView.reset(sf::FloatRect(0,0,viewSettings.maxX,viewSettings.maxY));                                     //On remet la vue a 0
                break;
        }
        /*#####*/

        /*RENDER*/
        gameWindow.clear(sf::Color(0,0,0,255));                                                                     //On efface la fenetre
        gameWindow.setView(gameView);
        switch(scene){                                                                                              //Pour chaque valeur de scene
            case SCENE_MAINMENU:                                                                                        //Si on est dans le menu principale
                scene = mainMenu.Update(mousePos,mouseLeftClick);                                                           //Mise a jour du menu principale
                break;
            case SCENE_GAME_EASY:                                                                                       //Si on est dans le jeux en mode facile
                if(!game)game = new Game(&gameWindow,&resourcesManager,&gameView);                                          //Si le jeux n'est pas encore créé => On créé le jeux
                scene = game->Update(SCENE_GAME_EASY,mousePos,mouseLeftClick,mouseRightClick,R_key);                        //Mise a jour du jeux
                break;
            case SCENE_GAME_MEDIUM:                                                                                     //Si on est dans le jeux en mode normal
                if(!game)game = new Game(&gameWindow,&resourcesManager,&gameView);                                          //Si le jeux n'est pas encore créé => On créé le jeux
                scene = game->Update(SCENE_GAME_MEDIUM,mousePos,mouseLeftClick,mouseRightClick,R_key);                      //Mise a jour du jeux
                break;
            case SCENE_GAME_MASTER:                                                                                     //Si on est dans le jeux en mode master
                if(!game)game = new Game(&gameWindow,&resourcesManager,&gameView);                                          //Si le jeux n'est pas encore créé => On créé le jeux
                scene = game->Update(SCENE_GAME_MASTER,mousePos,mouseLeftClick,mouseRightClick,R_key);                      //Mise a jour du jeux
                break;
            case SCENE_GAMEOVER:                                                                                        //Si le jeux est fini
                if(game){                                                                                                   //Si le jeux existe encore
                    score = game->GetScore();                                                                                   //Recuperation du score
                    delete game;                                                                                                //On supprime le jeux
                    game = NULL;                                                                                                //On retire l'adresse
                }
                scene = gameover.Update(mousePos,mouseLeftClick,score);                                                     //Mise a jour du gameover
                if(SCENE_GAMEOVER!=scene)clickLock=true;                                                                    //Si on change de scene => On bloque le clique de sourie
                break;
        }
        gameWindow.display();                                                                                       //On affiche la fenetre
        /*######*/
    }
    /*########*/


    /*CLEAN*/
    gameWindow.close();                                                                                             //On ferme la fenetre de jeux
    /*#####*/
}