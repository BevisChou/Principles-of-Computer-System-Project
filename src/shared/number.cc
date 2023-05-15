#include "number.hh"

Word int_to_bits(string x) {
    Word res;
    bool sign = false;
    int index = 0, length = x.length();
    int value = 0;
    //get the sign of the integer number
    if(x[0] == '-') {
        sign = true;
        index++;
    }
    //transform the input number from string to integer
    for(int i = index; i < length; ++i) {
        value *= 10;
        value += x[i] - '0';
    }
    //set the sign of the integer number
    if(sign)
        value = -value;
    //store the integer number to the Word variable bit by bit
    for(int i = 0; i < 32; ++i) {
        if(get_bit(value, i))
            res.set(i);
        else
            res.reset(i);
    }
    return res;
}

Word float_to_bits(string x) {
    Word res;
    vector<int> integer;
    vector<int> decimal;
    bool point = false;
    int index = 0, not_zero = 0, length = x.length();
    //set the sign of the float number
    if(x[0] == '-') {
        res.set(31);
        index++;
    }
    //get the integer part and the decimal part of the float number
    for(int i = index; i < length; ++i) {
        if (x[i] == '.')
            point = true;
        else {
            if (!point)
                integer.push_back(x[i]-'0');
            else
                decimal.push_back(x[i]-'0');
            not_zero += x[i]-'0';
        }
    }
    //deal with the special case: the input number is 0
    if(!(not_zero)) {
        for(int i = 0; i < 31; ++i)
            res.reset(i);
        return res;
    }
    //deal with the special case: value overflow
    if(integer.size() > 39) {
        for(int i = 0; i < 23; ++i)
            res.reset(i);
        for(int i = 23; i < 31; ++i)
            res.set(i);
        return res;
    }
    //transform the integer part of the input number from decimal string to binary string
    int sum = 1;
    int integer_count = 0;
    int buffer[132] = {0};
    while(sum) {
        sum = 0;
        for(unsigned int i = 0; i < integer.size(); ++i) {
            if(i == integer.size()-1)
                buffer[integer_count++] = integer[i] % 2;
            else
                integer[i+1] += (integer[i] % 2) * 10;
            integer[i] /= 2;
            sum += integer[i];
        }
    }
    //deal with the special case: value overflow
    if(buffer[129]+buffer[130]+buffer[131]) {
        for(int i = 0; i < 23; ++i)
            res.reset(i);
        for(int i = 23; i < 31; ++i)
            res.set(i);
        return res;
    }
    //set the exponent of the float number
    int exponent = 126 + integer_count;
    int remain = 23;
    for(int i = 0; i < 8; ++i) {
        if(get_bit(exponent, i))
            res.set(i+23);
        else
            res.reset(i+23);
    }
    //deal with the special case: integer accuracy overflow
    if(integer_count > 24) {
        int carry = 0;
        if(buffer[integer_count-25]) {
            carry = 1;
            for(int i = 0; i < 24; ++i) {
                if(buffer[integer_count-24+i]+carry > 1) {
                    buffer[integer_count-24+i] = 0;
                    carry = 1;
                }
                else {
                    buffer[integer_count-24+i] = 1;
                    carry = 0;
                    break;
                }
            }
            if(carry) {
                exponent++;
                for(int i = 0; i < 8; ++i) {
                    if(get_bit(exponent, i))
                        res.set(i+23);
                    else
                        res.reset(i+23);
                }
            }
        }
        for(int i = 0; i < 23; ++i) {
            if(buffer[integer_count-24+i+carry])
                res.set(i);
            else
                res.reset(i);
        }
        return res;
    }
    //deal with the special case: decimal accuracy overflow
    if(integer_count == 24) {
        int carry = 0;
        if((decimal.size() > 0) && (decimal[0] >= 5)) {
            carry = 1;
            for(int i = 0; i < 24; ++i) {
                if(buffer[i]+carry > 1) {
                    buffer[i] = 0;
                    carry = 1;
                }
                else {
                    buffer[i] = 1;
                    carry = 0;
                    break;
                }
            }
            if(carry) {
                exponent++;
                for(int i = 0; i < 8; ++i) {
                    if(get_bit(exponent, i))
                        res.set(i+23);
                    else
                        res.reset(i+23);
                }
            }
        }
        for(int i = 0; i < 23; ++i) {
            if(buffer[i+carry])
                res.set(i);
            else
                res.reset(i);
        }
        return res;
    }
    //deal with the special case: integer is 0, but decimal isn't
    if((integer_count == 1) && (buffer[0] == 0)) {
        int exponent = 127;
        bool significant = false;
        int carry = 0;
        while(!significant && exponent) {
            for(int i = decimal.size()-1; i >= 0; --i) {
                if(i == 0) {
                    if((decimal[i]*2+carry)/10)
                        significant = true;
                    int value = decimal[i] * 2 + carry;
                    decimal[i] = value % 10;
                    carry = 0;
                }
                else {
                    int value = decimal[i] * 2 + carry;
                    decimal[i] = value % 10;
                    carry = value / 10;
                }
            }
            exponent--;
        }
        for(int i = 0; i < 8; ++i) {
            if(get_bit(exponent, i))
                res.set(i+23);
            else
                res.reset(i+23);
        }
        int remain = 22;
        while(remain >= 0) {
            for(int i = decimal.size()-1; i >= 0; --i) {
                if(i == 0) {
                    if((decimal[i]*2+carry)/10)
                        res.set(remain);
                    else
                        res.reset(remain);
                    remain--;
                    int value = decimal[i] * 2 + carry;
                    decimal[i] = value % 10;
                    carry = 0;
                }
                else {
                    int value = decimal[i] * 2 + carry;
                    decimal[i] = value % 10;
                    carry = value / 10;
                }
            }
        }
        int rectify = 0;
        for(int i = decimal.size()-1; i >= 0; --i) {
            if((i == 0) && (decimal[i]*2+carry)/10) {
                rectify = 1;
            }
            else {
                int value = decimal[i] * 2 + carry;
                decimal[i] = value % 10;
                carry = value / 10;
            }
        }
        if(rectify) {
            carry = 1;
            for(int i = 0; i < 24; ++i) {
                if(res[i]) {
                    res.reset(i);
                    carry = 1;
                }
                else {
                    res.set(i);
                    carry = 0;
                    break;
                }
            }
            if(carry) {
                exponent++;
                for(int i = 0; i < 8; ++i) {
                    if(get_bit(exponent, i))
                        res.set(i+23);
                    else
                        res.reset(i+23);
                }
            }
        }
        return res;
    }
    //set the integer part of the float number
    remain = 23 - integer_count;
    for(int i = 0; i < integer_count-1; ++i) {
        if(buffer[integer_count-2-i])
            res.set(22-i);
        else
            res.reset(22-i);
    }
    //deal with the special case: no decimal part
    if(decimal.size() == 0) {
        while(remain >= 0) {
            res.reset(remain);
            remain--;
        }
        return res;
    }
    //set the decimal part of the float number
    int carry = 0;
    while(remain >= 0) {
        for(int i = decimal.size()-1; i >= 0; --i) {
            if(i == 0) {
                if((decimal[i]*2+carry)/10)
                    res.set(remain);
                else
                    res.reset(remain);
                remain--;
                int value = decimal[i] * 2 + carry;
                decimal[i] = value % 10;
                carry = 0;
            }
            else {
                int value = decimal[i] * 2 + carry;
                decimal[i] = value % 10;
                carry = value / 10;
            }
        }
    }
    int rectify = 0;
    for(int i = decimal.size()-1; i >= 0; --i) {
        if((i == 0) && (decimal[i]*2+carry)/10) {
            rectify = 1;
        }
        else {
            int value = decimal[i] * 2 + carry;
            decimal[i] = value % 10;
            carry = value / 10;
        }
    }
    if(rectify) {
        carry = 1;
        for(int i = 0; i < 24; ++i) {
            if(res[i]) {
                res.reset(i);
                carry = 1;
            }
            else {
                res.set(i);
                carry = 0;
                break;
            }
        }
        if(carry) {
            exponent++;
            for(int i = 0; i < 8; ++i) {
                if(get_bit(exponent, i))
                    res.set(i+23);
                else
                    res.reset(i+23);
            }
        }
    }
    return res;
}

string bits_to_int(Word x) {
    //allocate space for the integer string
    string res = (char*)malloc(12);
    for(int i = 0; i < 12; ++i)
        res[i] = 0;
    std::stack<char> char_stack;
    int index = 0;
    int value;
    //store the input number to a integer variable bit by bit
    for(int i = 0; i < 32; ++i) {
        if(x[i])
            set_bit(value, i);
        else
            reset_bit(value, i);
    }
    //deal with the special case: the input number is 0
    if(value == 0) {
        res[index] = '0';
        return res;
    }
    //set the sign of the integer number
    if(value < 0) {
        value = -value;
        res[index++] = '-';
    }
    //transform the integer number into a decimal string
    while(value) {
        char digit = value % 10;
        value /= 10;
        char_stack.push(digit+'0');
    }
    while(char_stack.size()) {
        res[index++] = char_stack.top();
        char_stack.pop(); 
    }
    return res;
}

string bits_to_float(Word x) {
    //allocate space for the float string
    string res = (char*)malloc(50);
    for(int i = 0; i < 50; ++i)
	        res[i] = 0;
    int res_pool[50] = {0};
    int index = 0, not_zero = 0, pool_index = 0;
    int exponent = 0;
    //set the sign of the float number
    if(x[31]) {
        res[index++] = '-';
    }
    for(int i = 0; i < 8; ++i) {
        if(x[i+23])
            set_bit(exponent, i);
        else
            reset_bit(exponent, i);
        not_zero += x[i+23];
    }
    //deal with the special case: the input number is 0
    if(!(not_zero)) {
        for(int i = 0; i < 23; ++i)
            not_zero += x[i];
        if(!(not_zero)) {
            res[index++] = '0';
            res[index++] = '.';
            for(int i = 0; i < 6; ++i)
                res[index++] = '0';
            return res;
        }
    }
    //deal with the special case: the integer part of the input number is 0
    int remain = 22;
    exponent -= 127;
    if(exponent < 0) {
        res_pool[pool_index++] = 0;
		res_pool[pool_index++] = '.';
		int decimal[40] = {0};
        while(remain >= 0) {
            decimal[39] = 1 * x[22-remain];
            for(int i = 39; i >= 0; --i) {
                if(i == 0)
                    decimal[0] = decimal[0] / 2;
                else
                    decimal[i-1] += (decimal[i] % 2) * 10;
                decimal[i] /= 2;
            }
            remain--;
        }
        decimal[39] = 1;
        while(exponent < 0) {
            for(int i = 39; i >= 0; --i) {
                if(i == 0)
                    decimal[0] = decimal[0] / 2;
                else
                    decimal[i-1] += (decimal[i] % 2) * 10;
                decimal[i] /= 2;
            }
            exponent++;
        }
        for(int i = 0; i < 6; ++i)
            res_pool[pool_index++] = decimal[38-i];
        int rectify = 0;
        for(int i = 31; i >= 0; --i)
            rectify += decimal[i];
        if((decimal[32] > 5) || ((decimal[32] == 5) && rectify) || ((decimal[32] == 5) && !rectify && (decimal[33]%2))) {
            rectify = 1;
            for(int i = pool_index-1; i >= 0; --i) {
                if(res_pool[i] == '.')
                    continue;
                res_pool[i] += rectify;
                if(res_pool[i] < 10) {
                    rectify = 0;
                    break;
                }
                else {
                    res_pool[i] = 0;
                    rectify = 1;
                }
            }
            if(rectify)
                res[index++] = '1';
        }
        for(int i = 0; i < pool_index; ++i) {
			if(res_pool[i] == '.')
			   	res[index++] = '.';
			else
			    res[index++] = res_pool[i] + '0';
		}
		return res;
    }
    //set the integer part of the float number
    else {
        int buffer[40] = {0};
        int length = 23;
        if(exponent < 23)
            length = exponent;
        remain -= length;
        buffer[0] = 1;
        int carry = 0;
        for(int i = 0; i < length; ++i) {
            carry = 1 * x[22-i];
            for(int bit = 0; bit < 40; ++bit) {
                int value = buffer[bit] * 2 + carry;
                buffer[bit] = value % 10;
                carry = value / 10;
            }
        }
        int loop = exponent - length;
        for(int i = 0; i < loop; ++i) {
            carry = 0;
            for(int bit = 0; bit < 40; ++bit) {
                int value = buffer[bit] * 2 + carry;
                buffer[bit] = value % 10;
                carry = value / 10;
            }
        }
        int position = 39;
        while(!buffer[position] && position)
            position--;
        if(!position && !buffer[position]) {
            res_pool[pool_index++] = 0;
            res_pool[pool_index++] = '.';
        }
        else {
            for(int i = position; i >= 0; --i)
                res_pool[pool_index++] = buffer[i];
            res_pool[pool_index++] = '.';
        }
    }
    //set the decimal part of the float number
    int limit = remain;
    if(remain < 0) {
    	for(int i = 0; i < pool_index; ++i) {
		    if(res_pool[i] == '.')
		       	res[index++] = '.';
		    else
			    res[index++] = res_pool[i] + '0';
		}
		for(int i = 0; i < 6; ++i)
		    res[index++] = '0';
		return res;
	}
    int decimal[25] = {0};
    while(remain >= 0) {
        decimal[24] = 1 * x[limit-remain];
        for(int i = 24; i >= 0; --i) {
            if(i == 0)
                decimal[0] = decimal[0] / 2;
            else
                decimal[i-1] += (decimal[i] % 2) * 10;
            decimal[i] /= 2;
        }
        remain--;
    }
    for(int i = 0; i < 6; ++i)
        res_pool[pool_index++] = decimal[23-i];
    int rectify = 0;
    for(int i = 16; i >= 0; --i)
        rectify += decimal[i];
    if((decimal[17] > 5) || ((decimal[17] == 5) && rectify) || ((decimal[17] == 5) && !rectify && (decimal[18]%2))) {
        rectify = 1;
        for(int i = pool_index-1; i >= 0; --i) {
            if(res_pool[i] == '.')
                continue;
            res_pool[i] += rectify;
            if(res_pool[i] < 10) {
                rectify = 0;
                break;
            }
            else {
                res_pool[i] = 0;
                rectify = 1;
            }
        }
        if(rectify)
            res[index++] = '1';
    }
    for(int i = 0; i < pool_index; ++i) {
	    if(res_pool[i] == '.')
	       	res[index++] = '.';
	    else
		    res[index++] = res_pool[i] + '0';
	}
    return res;
}
