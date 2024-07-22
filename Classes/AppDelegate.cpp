#include "AppDelegate.h"

using namespace cocos2d; // USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
	delete game;
}

bool AppDelegate::applicationDidFinishLaunching() {

	Director* director = Director::getInstance();
	GLView* glView = director->getOpenGLView();

	Vec2 dimensions = Vec2(1280, 960);

	if(!glView) {
		glView = GLViewImpl::createWithRect("Mills by Yelfra", Rect(0, 0, dimensions.x, dimensions.y));
		director->setOpenGLView(glView);
	}

	Board* board = Board::create();
	game = new Game(board);

	director->runWithScene(board);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}