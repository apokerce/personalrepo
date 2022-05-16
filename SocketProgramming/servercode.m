server = tcpserver('127.0.0.1', 3005,"ConnectionChangedFcn",@connectionFcn);
function connectionFcn(src, ~)
    data_array = [3 5 7 1 4 1 8 2 3 0];
    while src.Connected
        if (src.NumBytesAvailable>0)
            data = read(src,src.NumBytesAvailable,"string");        
            parsed_message = split(data,["=",";"]);
            parse_indices = split(parsed_message(4),",");
            parse_data = split(parsed_message(6),",");
            length_indices = length(parse_indices);
            back_message = parsed_message(1) + "=" + parsed_message(2) +";" + parsed_message(3)+ "=" + parsed_message(4) +";"+ parsed_message(5);
            if(parsed_message(2)=="GET")
                get = "=";
                for i=1:length_indices
                    get = get+num2str(data_array(str2double(parse_indices(i))+1));
                    if (length_indices>1 && i~=length_indices)
                        get = get + ',';
                    end
                end
                back_message = back_message + get + ";";
                write(src, back_message);
            elseif(parsed_message(2) == "PUT")
                get = "=";
                for i=1:length_indices
                     data_array(str2double(parse_indices(i))+1) = parse_data(i);
                end
                for i=1:10
		            get = get+num2str(data_array(i));
                    if (i~=10)
                        get = get + ',';
                    end 
                end
                back_message = back_message + get + ";";
                write(src, back_message);    
            elseif(parsed_message(2)=="CLR")
                get = "=";
                for i=1:10
                    data_array(i) = 0;
                end
                for i=1:10
		            get = get+num2str(data_array(i));
                    if (i~=10)
                        get = get + ',';
                    end 
                end
                back_message = back_message + get + ";";
                write(src, back_message);
            elseif(parsed_message(2)=="ADD")
                get = "=";
                x = 0;
                for i=1:length_indices
                    x = x + data_array(str2double(parse_indices(i))+1);
                end
                get = get + num2str(x);
                back_message = back_message + get + ";";
                write(src, back_message);
            end 
            disp("Server Array");
            disp(data_array);
        end
    end
end
