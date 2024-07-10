#include <Geode/Geode.hpp>

using namespace geode::prelude;
//#define m_disableShakeToggled 
#include <Geode/modify/LevelInfoLayer.hpp>
bool val = false;
void setlevel(auto level, bool val) {
	#ifdef m_disableShakeToggled
		level->m_disableShakeToggled = val;
	#else
		level->m_k112 = val;
	#endif
};

bool getval(auto level) {
	#ifdef m_disableShakeToggled
		return level->m_disableShakeToggled;
	#else
		return level->m_k112;
	#endif
};

class $modify(LevelInfoLayer) {
	void levelDownloadFinished(GJGameLevel* level) {
		LevelInfoLayer::levelDownloadFinished(level);
		setlevel(level,val);
	};
	bool init(GJGameLevel* level, bool thechallange) {
			bool init = LevelInfoLayer::init(level,thechallange);
				if (Mod::get()->getSettingValue<bool>("Auto-NoShake")) { 
					bool seen = Mod::get()->getSavedValue<bool>(fmt::format("seen_{}",level->m_levelID.value())) == true;
					if (seen) {
						bool saved = Mod::get()->getSavedValue<bool>(fmt::format("auto_{}",level->m_levelID.value()));
						setlevel(level,saved);
						log::debug("loaded 3");
					} else {
						log::debug("loaded 2");
						setlevel(level,true);
						Mod::get()->setSavedValue(fmt::format("seen_{}",level->m_levelID.value()), true);
						
					}
				} else {
					log::debug("loaded");
						setlevel(level,Mod::get()->getSavedValue<bool>(fmt::format("{}",level->m_levelID.value())) || false);
				}
				val = getval(this->m_level);
				log::debug("loaded {}",getval(this->m_level) );
			return init;
        }

	
	void onBack(CCObject* Sender) {
		Mod::get()->setSavedValue(fmt::format("{}",this->m_level->m_levelID.value()), getval(this->m_level));
		if (Mod::get()->getSettingValue<bool>("Auto-NoShake")) { 
			Mod::get()->setSavedValue(fmt::format("auto_{}",this->m_level->m_levelID.value()), getval(this->m_level));
		}
		log::debug("loaded {}",getval(this->m_level) );
		LevelInfoLayer::onBack(Sender);
	};
	void onPlay(CCObject* Sender) {
		Mod::get()->setSavedValue(fmt::format("{}",this->m_level->m_levelID.value()), getval(this->m_level));
		if (Mod::get()->getSettingValue<bool>("Auto-NoShake")) { 
			Mod::get()->setSavedValue(fmt::format("auto_{}",this->m_level->m_levelID.value()), getval(this->m_level));
		}
		LevelInfoLayer::onPlay(Sender);
	}

};

