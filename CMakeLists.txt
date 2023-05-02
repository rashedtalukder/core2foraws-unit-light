#  Copyright 2023 Rashed Talukder. All Rights Reserved.
#  Additions Copyright 2016 Espressif Systems (Shanghai) PTE LTD
#  
#  Licensed under the Apache License, Version 2.0 (the "License").
#  You may not use this file except in compliance with the License.
#  A copy of the License is located in the "license" file accompanying this 
#  file. This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
#  CONDITIONS OF ANY KIND, either express or implied. See the License for the 
#  specific language governing permissions and limitations under the License.

cmake_minimum_required( VERSION 3.16.0 )

set( COMPONENT_SRCDIRS . )
set( COMPONENT_ADD_INCLUDEDIRS "./include" )
set( COMPONENT_REQUIRES "Core2-for-AWS-IoT-Kit")

register_component()