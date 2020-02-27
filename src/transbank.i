%include "enums.swg"
%module TransbankWrap
%{
#include "hex_codes.h"
#include "responses.h"
#include "transbank_serial_utils.h"
#include "transbank.h"
%}

/* Force the generated Java code to use the C enum values rather than making a JNI call */
%javaconst(1);

%include "hex_codes.h"
%include "responses.h"
%include "transbank_serial_utils.h"
%include "transbank.h"
