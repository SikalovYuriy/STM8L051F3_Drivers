/* =================================================================================
 File name:     Driver_Interrupt.hpp                                
 Creation date: 21-05-2014
 Description:   
================================================================================== */

#ifndef _DRIVERINTERRUPT_H_
#define _DRIVERINTERRUPT_H_

//----------------------------------------------------------------------------------
#define STRINGVECTOR(x) #x
#define VECTOR_ID(x) STRINGVECTOR( vector = (x) )

#define __Interrupt_handler( Function_name, Vector_number )  \
_Pragma( VECTOR_ID(Vector_number) ) \
__interrupt void (Function_name)( void )

//----------------------------------------------------------------------------------

#endif

