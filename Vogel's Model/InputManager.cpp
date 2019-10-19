#include "InputManager.h"

void InputManager::registerCallback(KeyboardKey key, KeyCallback cb)
{
	// Record for the key will be created if it was not present
	m_keyCallbacks[key].push_back(cb);
}

void InputManager::clearCallbacks(KeyboardKey key)
{
	if (auto it = m_keyCallbacks.find(key); it != m_keyCallbacks.end())
		it->second.clear();
}

void InputManager::keyInput(KeyboardKey key)
{
	if (auto it = m_keyCallbacks.find(key); it != m_keyCallbacks.end())
		call(it->second);
}

void InputManager::call(const KeyCallbackList& callbacks)
{
	for (const auto& callback : callbacks)
		callback();
}
