#pragma once
#include <functional>
#include <SFML/Window.hpp>

class InputManager
{
public:
	using KeyboardKey = sf::Keyboard::Key;
	using KeyCallback = std::function<void()>;
	using KeyCallbackList = std::vector<KeyCallback>;
	using KeyCallbackMap = std::unordered_map<KeyboardKey, KeyCallbackList>;

	void registerCallback(KeyboardKey k, KeyCallback cb);
	void clearCallbacks(KeyboardKey k);
	void keyInput(KeyboardKey key);
private:
	static void call(const KeyCallbackList& callbacks);

	KeyCallbackMap m_keyCallbacks;
};
