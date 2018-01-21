#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <string>
#include <map>

const std::vector<std::string> tests{ 
	"++++++++[>+>++++<<-]>++>>+<[-[>>+<<-]+>>]>+[-<<<[->[+[-]+>++>>>-<<]<[<]>>++++++[<<+++++>>-]+<<++.[-]<<]>.>+[>>]>+]",
	// "Hello world!\n"
	"+++++ +++++             (initialize counter (cell #0) to 10)[                       (use loop to set the next four cells to 70/100/30/10)    > +++++ ++          (    add  7 to cell #1)    > +++++ +++++       (    add 10 to cell #2 )    > +++               (    add  3 to cell #3)    > +                 (    add  1 to cell #4)    <<<< -              (    decrement counter (cell #0))]                   > ++ .                  (print 'H')> + .                   (print 'e')+++++ ++ .              (print 'l').                       (print 'l')+++ .                   (print 'o')> ++ .                  (print ' ')<< +++++ +++++ +++++ .  (print 'W')> .                     (print 'o')+++ .                   (print 'r')----- - .               (print 'l')----- --- .             (print 'd')> + .                   (print bang)> .                     (print '\n')",
};

int eval(const std::string &);

int main()
{
	return eval(tests[1]);
}

int eval(const std::string &src)
{
	const unsigned cells{ 30000 };
	std::vector<int8_t> m(cells, 0);
	auto it = std::begin(m);
	std::stack<std::size_t> s;

	auto incr_cell = [&it]() -> void { (*it)++; };
	auto decr_cell = [&it]() -> void { (*it)--; };
	auto next_cell = [&it]() -> void { (it)++; };
	auto prev_cell = [&it]() -> void { (it)--; };
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
