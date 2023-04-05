#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

template <typename T> class SingletonHolder;
struct SDL_Window;

class WindowManager {
	public:
		bool init();
		SDL_Window* getWindow() const;
		WindowManager(const WindowManager&) = delete;
		WindowManager(WindowManager&&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;
		compl WindowManager();
		friend class SingletonHolder<WindowManager>;

	private:
		WindowManager();
		SDL_Window* m_window;
};

#endif