#ifndef APP_HPP
#define APP_HPP

typedef unsigned long long Uint64;

template<typename T> class SingletonHolder;

class App {
	public:
		void run();
	public:
		compl App();
		App(const App&) = delete;
		App(App&&) = delete;
		App& operator=(App&) = delete;
		friend class SingletonHolder<App>;

	private:
		App();

	private:
		bool init();
		void processInput();
		void update(Uint64);
		void render();
		void wake();
		void wait();
		void quit();
		void cleanup();

	private:
		Uint64 m_iStart, m_iEnd, m_iCurrentDelta, m_iTargetDelta, m_iFPS, m_iTimeSinceStarted;
		bool m_bStarted, m_bIsRun;
};

#endif