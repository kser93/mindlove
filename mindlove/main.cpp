#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <string>
#include <map>

const std::vector<std::string> tests{ 
	"++++++++[>+>++++<<-]>++>>+<[-[>>+<<-]+>>]>+[-<<<[->[+[-]+>++>>>-<<]<[<]>>++++++[<<+++++>>-]+<<++.[-]<<]>.>+[>>]>+]",
	"[,.]",
	"+++++ +++++             (initialize counter (cell #0) to 10)[                       (use loop to set the next four cells to 70/100/30/10)    > +++++ ++          (    add  7 to cell #1)    > +++++ +++++       (    add 10 to cell #2 )    > +++               (    add  3 to cell #3)    > +                 (    add  1 to cell #4)    <<<< -              (    decrement counter (cell #0))]                   > ++ .                  (print 'H')> + .                   (print 'e')+++++ ++ .              (print 'l').                       (print 'l')+++ .                   (print 'o')> ++ .                  (print ' ')<< +++++ +++++ +++++ .  (print 'W')> .                     (print 'o')+++ .                   (print 'r')----- - .               (print 'l')----- --- .             (print 'd')> + .                   (print bang)> .                     (print '\n')",
	"+[>+++++++++++++++++++++++++++++++++.]" // "Hello world!\n"
};
int eval(std::string);

int main()
{
	return eval(tests[1]);
}

int eval(std::string src)
{
	const unsigned cells{ 30000 };
	std::vector<int8_t> m(cells, 0);
	auto it = std::begin(m);
	std::stack<std::size_t> s;
	// BF program in string below prints  out

	for (auto inst_it = std::begin(src); inst_it != std::end(src); ++inst_it)
	{
		switch (*inst_it)
		{
		case '+': (*it)++; break;
		case '-': (*it)--; break;
		case '>': (it)++; break;
		case '<': (it)--; break;
		case '[':
			s.push(inst_it - std::begin(src));
			break;
		case ']':
			if (s.empty())
				return -1;
			if ((*it) != 0)
			{
				inst_it = std::begin(src) + s.top();
			}
			else
			{
				s.pop();
			}
			break;
		case '.': std::cout << *it; break;
		case ',': std::cin >> *it; break;
			// process unknown instructions by neglecting
		default: break;
		}
	}
	if (!s.empty())
		return -1;
	return 0;
}