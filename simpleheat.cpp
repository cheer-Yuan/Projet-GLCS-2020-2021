#include <mpi.h>
#include<time.h>
#include "commandlineconfig.hpp"
#include "finitediffheatsolver.hpp"
#include "fixedconditions.hpp"
#include "simulation.hpp"
#include "screenprinter.hpp"
#include "programoptionsconfig.hpp"
#include "hdf5data.hpp"

using namespace 
int main( int argc, char* argv[] )
{
    clock_t start = clock();
	// initialize the MPI library
	MPI_Init( &argc, &argv );

/*
	// Construct the command-line arguments parser
	CommandLineConfig config( argc, argv );
*/
    //use the new configuration library which reads a configuration file
    BoostConfig config( argc, argv );

	// construct the heat equation solver
	FinitediffHeatSolver heat_solver( config );
	// construct the initial condition setter
	FixedConditions init;
	// construct the simulation runner
	Simulation simulation( MPI_COMM_WORLD, config, heat_solver, init );
	// Add a printer to screen to observe the simulation
	//ScreenPrinter printer;
	//simulation.observe( printer );

	hdf5data store;
	simulation.observe(store);

	// run the simulation
	simulation.run();

	// finalize MPI
	MPI_Finalize();

    clock_t end = clock();

    float t =  end - start;
    cout << t << endl;
	return 0;
}
