Nie można używać gotowych rozwiązań, również w zakresie struktur danych (typu std::vector,std::list).
Jeśli takie rozwiązania są potrzebne, trzeba je stworzyć samemu, od zera, z ręcznym
zarządzaniem pamięcią.

будем юзать шаблоны потому шоб не переписывать инты на флоты на стринги разных структурах (массивы, списки)

Флаги -Wall -Wextra -Werror с первой строчки кода.
Пункт 6 гласит, что любое предупреждение компилятора — это ошибка (благодаря -Werror), и код просто не соберется. Настрой CMake сразу с этими флагами и фикси ворнинги по мере написания. Оставишь на потом — получишь стену из сотен ошибок за день до дедлайна.

CRITICAL CONSTRAINT - MEMORY & DATA STRUCTURES: > Strictly forbidden to use any C++ Standard Template Library (STL) containers (e.g., std::vector, std::list, std::stack, std::queue, std::unique_ptr, std::shared_ptr). You MUST implement all required data structures (arrays, linked lists, queues, stacks, trees) from scratch. Memory management must be 100% manual using raw pointers with new and delete (or new[] / delete[]). Every custom data structure MUST have a robust, leak-proof destructor (adhering to the Rule of Three/Five) that recursively or iteratively frees all allocated nodes/elements. The code will be stress-tested with hundreds of thousands of elements in a loop; any memory leak will result in an Out-of-Memory (OOM) crash and project failure.

CRITICAL CONSTRAINT - CLI PARSING & AUTOMATED GRADING API:
It is strictly forbidden to implement any custom command-line argument parsing logic (e.g., manual argc/argv parsing, getopt, boost::program_options). The application MUST exclusively use the provided parameters.h library and its Parameters namespace.

Execution Control: All program states (run mode, algorithm selection, data structure, data type, input/output paths) must be driven solely by the variables populated via Parameters::readParameters(argc, argv).

Immutable API: The parameters.h file is strictly read-only. Do not modify, extend, or alter any enum class definitions or methods inside it.

Zero-Tolerance Compliance: The professor uses automated scripts mapped directly to this specific API. Any custom interface, interactive console prompts (std::cin), or failure to map the enums correctly will break the evaluation scripts and result in an immediate fail (0 points).
