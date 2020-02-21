#ifndef LABELCELL_H
#define LABELCELL_H


std::ostream &operator<<(std::ostream &cout, std::map< std::string, unsigned int > &label_stack)
{
    int n = 13;
    std::cout << '\n';
    cout << '|' << std::setfill('=') << std::setw(n * 5 - 1) << '|' << std::endl;
    cout << '|' << " LABEL SEGMENT:                                                |\n";

    cout << '|' << std::setfill('=') << std::setw(n * 5 - 1) << '|' << std::endl;
    cout << '|' << std::setfill(' ') << std::setw(n) << "addr (int)|"  
         << std::setw(n) << " addr (hex)|" 
         << std::setw(3 * n ) << "label  |\n"
         << '|' << std::setfill('=') << std::setw(n * 5 - 1) << "|" 
         << std::setfill(' ' ) << std::endl;

    for (auto element : label_stack)
    {
        cout << '|' << std::setw(n-1) << element.second << '|'
             << std::setw(n-1) << ui_to_hex(element.second) << '|'
             << std::setw(3*n - 4) << element.first << "  |"
             << std::endl;
    }
    
    cout << '|' << std::setfill('=') << std::setw(n * 5) << "|\n" << std::endl;
    return cout;
}


#endif 