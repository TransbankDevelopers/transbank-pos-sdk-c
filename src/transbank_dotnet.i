%include "enums.swg"
%module TransbankWrap
%{
#include "hex_codes.h"
#include "tbk_baud_rate.h"
#include "responses.h"
#include "transbank_serial_utils.h"
#include "transbank.h"
%}

%include "hex_codes.h"
%include "tbk_baud_rate.h"
%include "responses.h"
%include "transbank_serial_utils.h"
%include "transbank.h"
