// standard C++ library headers
#include <chrono>
#include <iostream>
#include <thread>


// the implemented class (last)
#include "screenprinter.hpp"

using std::cout;
using std::endl;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void ScreenPrinter::simulation_updated( const Distributed2DField& data )
{
	if ( data.distribution().rank() == 0 ) {
		cout << "at t="<<data.time()<<" : [" << endl;
	}
	cout<<flush; 
	MPI_Barrier(data.distribution().communicator());
	sleep_for(milliseconds(1));
	for ( int pyy = data.distribution().extent( DY )-1; pyy >=0 ; --pyy ) {
		for ( int yy = data.noghost_view().extent( DY )-1; yy >=0 ; --yy ) {
			for ( int pxx = 0; pxx < data.distribution().extent( DX ); ++pxx ) {
				if ( data.distribution().coord( DX ) == pxx && data.distribution().coord( DY ) == pyy ) {
					if ( pxx == 0 ) {
						cout << "  [";
					}
					for ( int xx = 0; xx < data.noghost_view().extent( DX ); ++xx ) {
						if ( 0 == data.noghost_view(yy, xx) ) {
							cout << " .";
						} else {
							cout << " " << data.noghost_view(yy, xx);
						}
					}
					if ( pxx == data.distribution().extent( DX ) - 1 ) {
						cout << " ]\n";
					}
				}
				cout<<""<<flush;
				MPI_Barrier(data.distribution().communicator());
				sleep_for(milliseconds(1));
			}
		}
	}
	if ( data.distribution().rank() == 0 ) {
		cout << "]" << endl;
	}
	cout<<flush;
	MPI_Barrier(data.distribution().communicator());
	sleep_for(milliseconds(1));
}

/***************************** HDF5 ****************************/

void ScreenPrinter::simulation_updated( const Distributed2DField& data )
{

	//declarer tableau pour stocker le resultat localement
	double *resultat[data.distribution().extent( DX )][data.distribution().extent( DY )];

	//stocker les resultats

	
	if ( data.distribution().rank() == 0 ) {
		cout << "at t="<<data.time()<<" : [" << endl;
	}
	cout<<flush; 
	MPI_Barrier(data.distribution().communicator());
	sleep_for(milliseconds(1));
	for ( int pyy = data.distribution().extent( DY )-1; pyy >=0 ; --pyy ) {
		for ( int yy = data.noghost_view().extent( DY )-1; yy >=0 ; --yy ) {
			for ( int pxx = 0; pxx < data.distribution().extent( DX ); ++pxx ) {
				if ( data.distribution().coord( DX ) == pxx && data.distribution().coord( DY ) == pyy ) {
					if ( pxx == 0 ) {
						cout << "  [";
					}
					for ( int xx = 0; xx < data.noghost_view().extent( DX ); ++xx ) {
						if ( 0 == data.noghost_view(yy, xx) ) {
							cout << " .";
						} else {
							cout << " " << data.noghost_view(yy, xx);
						}
					}
					if ( pxx == data.distribution().extent( DX ) - 1 ) {
						cout << " ]\n";
					}
				}
				cout<<""<<flush;
				MPI_Barrier(data.distribution().communicator());
				sleep_for(milliseconds(1));
			}
		}
	}
	if ( data.distribution().rank() == 0 ) {
		cout << "]" << endl;
	}
	cout<<flush;
	
		const string file_name = "my_file.h5";
	
	// create the file access property list
	const hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
	H5Pset_fapl_mpio(fapl, MPI_COMM_WORLD, MPI_INFO_NULL);
		
	// create the file
	const hid_t h5file = H5Fcreate(file_name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, fapl);
	
	// create the file dataspace
	int mpi_size; MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	const vector<hsize_t> file_dims { data.distribution().extent( DX ), data.distribution().extent( DY )*mpi_size };
	const hid_t file_space = H5Screate_simple(file_dims.size(), file_dims.data(), NULL);

	// create the dataset
	const hid_t dataset = H5Dcreate(h5file, "data", H5T_NATIVE_DOUBLE, file_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// select the region we want to write in the file dataspace
	const vector<hsize_t> file_start { 0, rank*data.distribution().extent( DY ) };
	const vector<hsize_t> file_count { data.distribution().extent( DX ), data.distribution().extent( DY ) };
	H5Sselect_hyperslab(file_space, H5S_SELECT_SET, file_start.data(), NULL, file_count.data(), NULL );

	// create the memory dataspace
	const vector<hsize_t> mem_dims { data.distribution().extent( DX ), full_data.distribution().extent( DY ) };
	const hid_t mem_space =  H5Screate_simple(mem_dims.size(), mem_dims.data(), NULL);
	
	// select the region we want to write in the memory dataspace
	const vector<hsize_t> mem_start { 0, ghost };
	const vector<hsize_t> mem_count { data.distribution().extent( DX ), data.distribution().extent( DY ) };
	H5Sselect_hyperslab(mem_space, H5S_SELECT_SET, mem_start.data(), NULL, mem_count.data(), NULL );
	
	// create the data transfer property list
	const hid_t xfer_plist = H5Pcreate(H5P_DATASET_XFER);
	H5Pset_dxpl_mpio(xfer_plist, H5FD_MPIO_COLLECTIVE);
	
	// write data to HDF5!
	H5Dwrite(dataset, H5T_NATIVE_DOUBLE, mem_space, file_space, xfer_plist, data.data());
	
	H5Pclose(fapl);
	H5Pclose(xfer_plist);
	H5Sclose(mem_space);
	H5Dclose(dataset);
	H5Sclose(file_space);
	H5Fclose(h5file);





	MPI_Barrier(data.distribution().communicator());
	sleep_for(milliseconds(1));
}