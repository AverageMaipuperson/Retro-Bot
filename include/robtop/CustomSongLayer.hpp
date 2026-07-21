#pragma once

#include <cocos2d.h>
#include "CCBlockLayer.h"
#include "cocos2dExt.h"
#include "LevelSettingsObject.hpp"
#include <string>
#include <vector>
#include <mutex>
#include <atomic>

using namespace cocos2d;
using namespace cocos2d::extension;
class VerticalScissorNode;

struct SongInfo {
	std::string id, name, author, size, length, verified, downloads;
};

class CustomSongLayer : public CCBlockLayer {

public:
	static CCSprite* m_loadingCircle;
	static std::mutex songDataMutex;
	static std::string s_currentSongID;
	static std::string s_currentSongName;
	static std::vector<SongInfo> songData;
	static std::vector<std::string> songList;
    static std::vector<std::string> songIDs;
	static volatile bool s_hasVersionData;
	static volatile bool s_failed;
	static volatile int i_dispID;
	static volatile int s_errorType;
	static std::string c_fullPath;
	static std::string c_fullLink;
	static volatile int i_res;
	static volatile int s_httpError;
	static volatile int i_fperrno;
	static CCNode* m_contentHolder;
	static CCNode* m_container;
	static CCMenu* m_downloadBtn;
	static CCMenu* m_downloadingBtn;
	static CCMenu* m_useBtn;
	static float s_currentProgress;
	static CCMenu* s_currentActiveStopBtn;
	static CCMenu* s_currentActivePlayBtn;
	static CCMenu* s_currentActiveUseBtn;
	static CCMenu* s_currentActiveUseOnBtn;
	static bool b_hasSelectedSong;
	static std::map<int, float> m_progresses;
	static std::mutex m_progressMutex;
	static int i_currentUsedID;
	static LevelSettingsObject* m_lso;
	CCLayer* m_self;
	static CCMenu* m_refreshBtn;
	static CCScrollView* m_scroll;
	cocos2d::CCPoint m_bgOrigin;
    cocos2d::CCSize  m_bgSize;
    VerticalScissorNode* m_scissorNode = nullptr;
	CCScale9Sprite* m_bgPanel;
	static std::atomic<int> m_orderType;
	static std::atomic<int> m_filter;
	static std::vector<std::string> m_sSongNames;
	std::map<int, std::string> sortedMap;
	virtual ~CustomSongLayer();
    static CustomSongLayer* create(CCLayer* referrer, LevelSettingsObject* object);
	void onClose(CCObject*);
	void loadCustomSongCell(int songID);
	void onSongClick(CCObject* pSender);
	void createSongList();
	void updateLoadCheck(float dt);
	void getSongs();
	void networkTimeout(float dt);
	void downloadFile(int);
	void incrementDownloadCount(int);
	void onDownload(CCObject*);
	void showDownloadError(int songID);
	void errorCheck(float dt);
	void error(int songID, CCLabelBMFont* label, int errorType);
	void updateProgressBar(float dt);
	void onDownloadFinished();
	void playAudio(CCObject* pSender);
	void stopAudio(CCObject* pSender);
	void use(CCObject* pSender);
	void changeSong(int);
	void deleteAudio(CCObject*);
	void setAllMenusEnabled(CCNode* parent, bool enabled);
	void onUploadPage();
	void onRefresh();
	void showSongInfo(CCObject*);
	void onFilter();
	// virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
	virtual void onEnter() override; 
protected:
    bool init(CCLayer* referrer, LevelSettingsObject* object);
    void keyBackClicked() override;
	cocos2d::CCSprite* m_scrollbarThumb; 
    float minY;
    float maxY;
    float desiredHeight;
};