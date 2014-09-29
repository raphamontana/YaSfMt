/**
 * \file main.cpp
 * \brief Yet another Structure from Motion toolkit
 *
 * \author Raphael Montanari
 * \copyright Laboratório de Aprendizado de Robôs
 * \version 1.0
 * \date 20/01/2013
 *
 * Main source file of the software.
 */

#include <cstdlib>
#include <iostream>
#include "core/Core.h"
//#include <kalman/ekfilter.hpp>

using namespace std;

/**
 * \brief Main routine of the application.
 *
 * Instantiate and execute the major system class.
 * \param[in] argc Argument count.
 * \param[in] argv Argument vector.
 * \return Return 0 on success or something else on failure.
 */
int main( int argc, char *argv[] )
{
    cout << (argc - 1) << " argument(s) passed to " << argv[0] << endl;

    Core application;
    try {
        application.exec();
    }
    catch ( const char* &ex ) {
        cerr << "Exception caught: " << ex << endl;
        return( EXIT_FAILURE );
    }

    cout << "All went right!" << endl;
    return( EXIT_SUCCESS );
}
