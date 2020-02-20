#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <algorithm>


class Vector {
public:
    using value_type = std::string;

    Vector (const size_t) {}

    void add (const std::string &str) { m_data.push_back(str); }

    void unstable_sort (bool (*cmp) (const std::string &a, const std::string &b) = nullptr) {
        if (cmp) {
            std::sort(m_data.begin(), m_data.end(), cmp);
        } else {
            std::sort(m_data.begin(), m_data.end());
        }
        m_end = m_data.end();
    }

    void stable_sort (bool (*cmp) (const std::string &a, const std::string &b) = nullptr) {
        if (cmp) {
            std::stable_sort(m_data.begin(), m_data.end(), cmp);
        } else {
            std::stable_sort(m_data.begin(), m_data.end());
        }
        m_end = m_data.end();
    }

    void unique () { m_end = std::unique(m_data.begin(), m_data.end()); }

    auto begin () const { return m_data.begin(); }

    auto end () const { return m_end; }

private:
    std::vector<std::string> m_data;
    typename std::vector<std::string>::const_iterator m_end;
};

using Lines = Vector;

template<class C>
void print_out (std::ostream &strm, const C &c, const bool reverse) {
    std::ostream_iterator<typename C::value_type> out(strm, "\n");
    if (!reverse) {
        std::copy(c.begin(), c.end(), out);
    } else {
        std::copy(std::make_reverse_iterator(c.end()),
                  std::make_reverse_iterator(c.begin()), out);
    }
}


bool StableStrToLowerCompare (const std::string &a, const std::string &b) {
    std::string lower_a = a, lower_b = b;
    std::transform(lower_a.begin(), lower_a.end(), lower_a.begin(), ::tolower);
    std::transform(lower_b.begin(), lower_b.end(), lower_b.begin(), ::tolower);
    return a.empty() || (!b.empty() && lower_a < lower_b);
}

bool UnstableStrToLowerCompare (const std::string &a, const std::string &b) {
    std::string lower_a = a, lower_b = b;
    std::transform(lower_a.begin(), lower_a.end(), lower_a.begin(), ::tolower);
    std::transform(lower_b.begin(), lower_b.end(), lower_b.begin(), ::tolower);;
    return a.empty() || (!b.empty() && lower_a <= lower_b);
}

bool StableStrToNumberCompare (const std::string &a, const std::string &b) {
    return a.empty() || (!b.empty() && std::strtol(a.data(), nullptr, 0) < std::strtol(b.data(), nullptr, 0));
}

bool UnstableStrToNumberCompare (const std::string &a, const std::string &b) {
    return a.empty() || (!b.empty() && std::strtol(a.data(), nullptr, 0) <= std::strtol(b.data(), nullptr, 0));
}

void sort_stream (std::istream &input,
                  const bool reverse_mode,
                  const bool unique_mode,
                  const bool ignore_case_mode,
                  const bool numeric_sort_mode,
                  const bool stable_mode) {
    // calculate size
    input.seekg(0, std::ios_base::end);
    const auto end_pos = input.tellg();
    input.seekg(0);

    Lines lines(end_pos);

    // read lines
    std::string line;
    while (std::getline(input, line)) {
        lines.add(line);
    }

    if (stable_mode) {
        if (ignore_case_mode) {
            lines.stable_sort(StableStrToLowerCompare);
        } else if (numeric_sort_mode) {
            lines.stable_sort(StableStrToNumberCompare);
        } else {
            lines.stable_sort();
        }
    } else {
        if (ignore_case_mode) {
            lines.unstable_sort(UnstableStrToLowerCompare);
        } else if (numeric_sort_mode) {
            lines.unstable_sort(UnstableStrToNumberCompare);
        } else {
            lines.unstable_sort();
        }
    }

    if (unique_mode) {
        lines.unique();
    }

    /*** Print in console ***/
    print_out(std::cout, lines, reverse_mode);
    /*** Print in file ***/
    //freopen("tests_ans/1_ans_test.out", "w", stdout);
    //print_out(std::cout, lines, reverse_mode);
}

int main (int argc, char **argv) {
    bool opts_finished = false,
            reverse_mode = false,
            unique_mode = false,
            ignore_case_mode = false,
            numeric_sort_mode = false,
            stable_mode = false;
    std::string_view file_input_name = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '\0') {
            continue;
        }

        if (argv[i][0] == '-' && !opts_finished) {
            if (argv[i] == std::string_view("--reverse")) {
                reverse_mode = true;
            } else if (argv[i] == std::string_view("--unique_mode")) {
                unique_mode = true;
            } else if (argv[i] == std::string_view("--ignore-case")) {
                ignore_case_mode = true;
            } else if (argv[i] == std::string_view("--numeric-sort")) {
                numeric_sort_mode = true;
            } else if (argv[i] == std::string_view("--stable_mode")) {
                stable_mode = true;
            } else {
                for (char short_args : std::string_view(argv[i] + 1)) {
                    switch (short_args) {
                        case '-':
                            opts_finished = true;
                            break;
                        case 'r':
                            reverse_mode = true;
                            break;
                        case 'u':
                            unique_mode = true;
                            break;
                        case 'f':
                            ignore_case_mode = true;
                            break;
                        case 'n':
                            numeric_sort_mode = true;
                            break;
                        case 's':
                            stable_mode = true;
                            break;
                    }
                }
            }
            continue;
        }
        file_input_name = std::string_view(argv[i]);
        break;
    }

    std::ifstream file_in;
    if (!file_input_name.empty()) {
        file_in.open(file_input_name.data());
        if (!file_in) {
            std::cerr << "Failed to open file '" << file_input_name << "'\n";
            return -1;
        }
    }
    sort_stream(!file_input_name.empty() ? file_in : std::cin,
                reverse_mode, unique_mode, ignore_case_mode,
                numeric_sort_mode, stable_mode);
    return 0;
}