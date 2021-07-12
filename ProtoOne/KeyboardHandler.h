#ifndef KEYBOARDHANDLER_H_
#define KEYBOARDHANDLER_H_
#include <unordered_map>
class KeyboardHandler {
	
public:	

	enum KeyState 
	{
		PRESSED,
		RELEASED,
		IDLE
	};

	KeyboardHandler() {};
	std::unordered_map<Uint8, KeyState> mKeyStates;
	
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

	//using [] messes me up
	bool isPressed(const Uint8 key) { 

		//move this search logic to another function
		//create soh cah toa functions in my math class
		bool rtnVal = false;
		std::unordered_map<Uint8, KeyState>::const_iterator got = mKeyStates.find(key);
		if (got == mKeyStates.end()){
		  //BUTTON NOT FOUND
		}
		else {
			//std::cout << "BOOGA1" << std::endl;
			if (mKeyStates.at(key) == PRESSED) { 
				//std::cout << "BOOGA2" << std::endl;
				rtnVal = true;
			}
		}
			
		return rtnVal;	 
		
	}
	bool isReleased(const Uint8 key) { 
		
		//move this search logic to another function
		//create soh cah toa functions in my math class
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
	void btnReleased(const Uint8 key) {	mKeyStates[key] = RELEASED; };	

};

#endif