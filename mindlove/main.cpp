#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <iterator>


const std::vector<std::string> tests{ 
	/*"Hello world!\n"*/
	"++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.",
	/*(Bounds check left : pipe to wc should display 1)*/
	"+[+++++++++++++++++++++++++++++++++.<]",
};


int eval(const std::string &);

template<class RandomAccessIt>
RandomAccessIt circ_next(RandomAccessIt, const RandomAccessIt &, const RandomAccessIt &);

template<class RandomAccessIt>
RandomAccessIt circ_prev(RandomAccessIt, const RandomAccessIt &, const RandomAccessIt &);

int main()
{
	return eval(tests[1]);
}

int eval(const std::string &src)
{
	const unsigned cells{ 30000 };
	std::vector<int8_t> m(cells, 0);
	auto it = std::begin(m);
	const auto m_first = std::begin(m);
	const auto m_last = std::end(m);
	std::stack<std::size_t> s;

	auto incr_cell = [&it]() -> void { (*it)++; };
	auto decr_cell = [&it]() -> void { (*it)--; };
	auto next_cell = [&it, &m_first, &m_last]() -> void
	{
		it = circ_next(it, m_first, m_last);
	};
	auto prev_cell = [&it, &m_first, &m_last]() -> void
	{
		it = circ_prev(it, m_first, m_last);
	};
	auto read_cell = [&it]() -> void { std::cin >> *it; };
	auto write_cell = [&it]() -> void { std::cout << *it; };

	auto open_cycle = [&s](const auto &first, const auto &current) -> void {
		s.push(std::distance(first, current));
	};
	auto close_cycle = [&s, &it](const auto &first, auto &current) -> void {
		if (s.empty())
			throw std::invalid_argument("Unbalanced brackets - nothing to close at symbol " + std::to_string(std::distance(first, current)));
		if ((*it) != 0)
		{
			current = first;
			std::advance(current, s.top());
		}
		else s.pop();
	};

	for (auto inst_it{ std::begin(src) }; inst_it != std::end(src); ++inst_it)
	{
		switch (*inst_it)
		{
		case '+': incr_cell(); break;
		case '-': decr_cell(); break;
		case '>': next_cell(); break;
		case '<': prev_cell(); break;
		case ',': read_cell(); break;
		case '.': write_cell(); break;
		case '[': open_cycle(std::begin(src), inst_it); break;
		case ']': 
			try 
			{
				close_cycle(std::begin(src), inst_it);
			}
			catch (std::invalid_argument &e)
			{
				throw e;
			}
			break;
		// process unknown instructions by neglecting
		default: break;
		}
	}
	if (!s.empty())
		throw std::invalid_argument("Unbalanced brackets - did not closed at the end");
	return 0;
}

template<class RandomAccessIt>
RandomAccessIt circ_next(RandomAccessIt it, const RandomAccessIt &first, const RandomAccessIt &last)
{
	if (it == last - 1)
		return first;
	else
		return std::next(it);
}

template<class RandomAccessIt>
RandomAccessIt circ_prev(RandomAccessIt it, const RandomAccessIt &first, const RandomAccessIt &last)
{
	if (it == first)
		return last - 1;
	else
		return std::prev(it);
}
