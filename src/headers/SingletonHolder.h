#ifndef SINGLETONHOLDER_H
#define SINGLETONHOLDER_H

template <typename T> class SingletonHolder {
	public:
		static T& s_instance;

		SingletonHolder() = default;
		SingletonHolder(const SingletonHolder&) = delete;
		SingletonHolder(SingletonHolder&&) = delete;
		SingletonHolder& operator=(SingletonHolder&) = delete;
		compl SingletonHolder() = default;
};

#endif