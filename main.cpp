#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>

class Node {
public:
    std::string date;
    Node *left = nullptr, *right = nullptr;
    Node *root = nullptr;
    ~Node() {
        delete right, left;
    }
};

Node *createNode(std::string _date, Node *tree) {
    Node *new_node = new Node();
    new_node->date = _date;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->root = *&tree->root;
    return new_node;
}

class Window {
private:
    //Window
        sf::ContextSettings settings;
        sf::RenderWindow *renderWindow;
    //Brackground
        sf::Sprite *background;
        sf::Texture *imgBackground;
    //Font
        sf::Font font;
    //QuestionAnswer
        sf::Text *questionAnswer;
    //Reset
        sf::Text *reset, *exit;
    //Buttons
        sf::Sprite *buttonYesQA, *buttonNopQA;
        sf::Texture imgButtonYesQA, imgButtonNopQA;
    //Event
        sf::Event eventWindow;
    //Tree
        Node *tree;
        int option, pointer, gameOver;
        char question[50], answer[50];
public:
    Window() {
        //Window
            settings.antialiasingLevel = 8;
            renderWindow = new sf::RenderWindow(sf::VideoMode(950, 450), "Akinator", sf::Style::Default, settings);
            renderWindow->setVerticalSyncEnabled(true);
        //Font
            font.loadFromFile("Font.ttf");
        //ButtonYES
            imgButtonYesQA.loadFromFile("Buttons/Yes.png");
            buttonYesQA = new sf::Sprite(imgButtonYesQA);
            buttonYesQA->scale(0.8, 0.8);
            buttonYesQA->setPosition(80, 240);
        //ButtonNOP
            imgButtonNopQA.loadFromFile("Buttons/No.png");
            buttonNopQA = new sf::Sprite(imgButtonNopQA);
            buttonNopQA->scale(0.8, 0.8);
            buttonNopQA->setPosition(280, 240);
        //Reset
            reset = new sf::Text("Reiniciar", font, 18);
            reset->setPosition(10, 420);
            reset->setFillColor(sf::Color(0, 0, 0));
        //Exit
            exit = new sf::Text("Salir", font, 18);
            exit->setPosition(130, 420);
            exit->setFillColor(sf::Color(0, 0, 0));
        //Tree
            tree = NULL;
            option = 0;
            pointer = 0;
            gameOver = 0;
            question[50] = {};
            answer[50] = {};
    }
    ~Window() {
        delete renderWindow;
        delete background;
        delete imgBackground;
        delete questionAnswer;
        delete reset;
        delete exit;
        delete buttonYesQA;
        delete buttonNopQA;
        delete tree;
    }
    void showWindow(Node *tree, int count) {
        system("cls");
        //Tree
            if (tree == NULL) {
                Node *new_nodeRoot = new Node();
                new_nodeRoot->date = "Es tu mascota";
                new_nodeRoot->right = NULL;
                new_nodeRoot->left = NULL;
                tree = new_nodeRoot;
                new_nodeRoot->root = *&tree;
                tree = new_nodeRoot;
            }
        //Question
            questionAnswer = new sf::Text(tree->date, font, 33);
            questionAnswer->setPosition(20, 150);
            questionAnswer->setFillColor(sf::Color(255, 255, 255));
        //LoopWindow
        while (renderWindow->isOpen()) {
            pointer = 0;
            option = 0;
            //Background
                if (gameOver == 1) {
                    imgBackground->loadFromFile("Background/Akinator3.png");
                }
                else if (tree->right == NULL && tree->left == NULL) {
                    imgBackground->loadFromFile("Background/Akinator1.png");
                }
                else {
                    imgBackground->loadFromFile("Background/Akinator2.png");
                }
                background = new sf::Sprite(*imgBackground);
            //Draw
                renderWindow->clear(sf::Color::Cyan);
                renderWindow->draw(*background);
                renderWindow->draw(*questionAnswer);
                renderWindow->draw(*reset);
                renderWindow->draw(*exit);
                renderWindow->draw(*buttonYesQA);
                renderWindow->draw(*buttonNopQA);
                renderWindow->display();
            //Pause
                if (gameOver == 1) {
                    Sleep(3000);
                    gameOver = 0;
                    showWindow(*&tree->root, 0);
                }
            //Events
                while (renderWindow->pollEvent(eventWindow)) {
                    if (eventWindow.type == sf::Event::MouseButtonReleased) {
                        if (eventWindow.mouseButton.button == sf::Mouse::Left) {
                            if(buttonYesQA->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*renderWindow)))) {
                                if (tree->right == NULL && tree->left == NULL) {
                                    gameOver = 1;
                                }
                                else {
                                    showWindow(tree->right, count+1);
                                }
                            }
                            else if(buttonNopQA->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*renderWindow)))) {
                                if (tree->right == NULL && tree->left == NULL) {
                                    //EnterQA
                                    std::cout << "\nEnter a question about your character:" <<  std::endl;
                                    fflush(stdin);
                                    std::cin.getline(question, 50);
                                    std::cout << "Enter the answer:" <<  std::endl;
                                    fflush(stdin);
                                    std::cin.getline(answer, 50);
                                    //AddNodes
                                    Node *new_nodeL = createNode(tree->date, *&tree->root);
                                    tree->left = new_nodeL;
                                    *&tree->date = question;
                                    Node *new_nodeR = createNode(answer, *&tree->root);
                                    tree->right = new_nodeR;
                                    //Reset
                                    gameOver = 0;
                                    showWindow(*&tree->root, 0);
                                }
                                else {
                                    showWindow(tree->left, count+1);
                                }
                            }
                            else if(reset->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*renderWindow)))) {
                                gameOver = 0;
                                showWindow(*&tree->root, 0);
                            }
                            else if(exit->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*renderWindow)))) {
                                goodbye();
                            }
                        }
                    }
                    if (eventWindow.type == sf::Event::Closed) {
                        renderWindow->close();
                    }
                }
        }
    }
    int goodbye() {
        renderWindow->close();
        std::cout << "\nThanks for playing." <<  std::endl;
        std::cout << "Created by Miguel Ventura." <<  std::endl;
        return EXIT_SUCCESS;
    }
};
int main() {
    system("cls");
    Window *window = new Window();
    window->showWindow(NULL, 0);
    return 0;
}