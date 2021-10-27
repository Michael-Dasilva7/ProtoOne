#ifndef KEYBOARDHANDLER_H_
#define KEYBOARDHANDLER_H_
#include <unordered_map>
class KeyboardHandler {

public:

	Uint8				mMaxKeyPresses;//if i create a max key press variable, i need to split up the release and press states lists, OR, have the max presses = max releases;

	enum KeyState
	{
		PRESSED,
		RELEASED,
		IDLE
	};


	std::unordered_map<Uint8, KeyState> mKeyStates;

	KeyboardHandler()
	{

		mMaxKeyPresses = 5;
	};

	void setIdleKeyState(const Uint8 key) {
		setKeyState(key, IDLE);
	}

	void setKeyState(const Uint8 key, KeyState s) {
		std::unordered_map<Uint8, KeyState>::const_iterator got = mKeyStates.find(key);
		if (got == mKeyStates.end()) {
			//BUTTON NOT FOUND
		}
		else {
			mKeyStates[key] = IDLE;
		}
	}

	bool isPressed(const Uint8 key) {
		bool rtnVal = false;
		
		//move this search logic to another function
		//if (mKeyStates.size() < mMaxKeyPresses) {
			std::unordered_map<Uint8, KeyState>::const_iterator got = mKeyStates.find(key);
			if (got == mKeyStates.end()) {
				//BUTTON NOT FOUND
			}
			else {
				if (mKeyStates.at(key) == PRESSED) {
					rtnVal = true;
				}
			}
		//}
		return rtnVal;

	}
	bool isReleased(const Uint8 key) {

		//move this search logic to another function
		bool rtnVal = false;
		std::unordered_map<Uint8, KeyState>::const_iterator got = mKeyStates.find(key);
		if (got == mKeyStates.end()) {}
		else {
			if (mKeyStates.at(key) == RELEASED) {
				rtnVal = true;
			}
		}
		return rtnVal;
	};

 	void btnPressed(const Uint8 key) { mKeyStates[key] = PRESSED; };
	void btnReleased(const Uint8 key) { mKeyStates[key] = RELEASED; };


	void clearButtons() {
		std::unordered_map<Uint8, KeyState>::const_iterator got;		
		mKeyStates.clear(); 

	};
};

#endif