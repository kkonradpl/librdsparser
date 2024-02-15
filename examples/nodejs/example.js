const koffi = require('koffi');
const path = require('path');
const os = require('os');
const win32 = (os.platform() == 'win32')
const unicode_type = (win32 ? 'int16_t' : 'int32_t')
const filename = 'librdsparser.' + (win32 ? 'dll' : 'so')
const lib = koffi.load(path.join(__dirname, filename));

koffi.proto('void callback_pi(void *rds, void *user_data)');
koffi.proto('void callback_pty(void *rds, void *user_data)');
koffi.proto('void callback_tp(void *rds, void *user_data)');
koffi.proto('void callback_ta(void *rds, void *user_data)');
koffi.proto('void callback_ms(void *rds, void *user_data)');
koffi.proto('void callback_ecc(void *rds, void *user_data)');
koffi.proto('void callback_country(void *rds, void *user_data)');
koffi.proto('void callback_af(void *rds, uint32_t af, void *user_data)');
koffi.proto('void callback_ps(void *rds, void *user_data)');
koffi.proto('void callback_rt(void *rds, int flag, void *user_data)');
koffi.proto('void callback_ptyn(void *rds, void *user_data)');
koffi.proto('void callback_ct(void *rds, void *ct, void *user_data)');

const rdsparser = {
    new: lib.func('void* rdsparser_new()'),
    free: lib.func('void rdsparser_free(void *rds)'),
    clear: lib.func('void rdsparser_clear(void *rds)'),
    parse_string: lib.func('bool rdsparser_parse_string(void *rds, const char *input)'),
    set_extended_check: lib.func('bool rdsparser_set_extended_check(void *rds, bool value)'),
    set_text_correction: lib.func('bool rdsparser_set_text_correction(void *rds, uint8_t text, uint8_t type, uint8_t error)'),
    set_text_progressive: lib.func('bool rdsparser_set_text_progressive(void *rds, uint8_t string, bool state)'),
    get_pi: lib.func('int32_t rdsparser_get_pi(void *rds)'),
    get_pty: lib.func('int8_t rdsparser_get_pty(void *rds)'),
    get_tp: lib.func('int8_t rdsparser_get_tp(void *rds)'),
    get_ta: lib.func('int8_t rdsparser_get_ta(void *rds)'),
    get_ms: lib.func('int8_t rdsparser_get_ms(void *rds)'),
    get_ecc: lib.func('int16_t rdsparser_get_ecc(void *rds)'),
    get_country: lib.func('int rdsparser_get_country(void *rds)'),
    get_ps: lib.func('void* rdsparser_get_ps(void *rds)'),
    get_rt: lib.func('void* rdsparser_get_rt(void *rds, int flag)'),
    get_ptyn: lib.func('void* rdsparser_get_ptyn(void *rds)'),
    register_pi: lib.func('void rdsparser_register_pi(void *rds, void *cb)'),
    register_pty: lib.func('void rdsparser_register_pty(void *rds, void *cb)'),
    register_tp: lib.func('void rdsparser_register_tp(void *rds, void *cb)'),
    register_ta: lib.func('void rdsparser_register_ta(void *rds, void *cb)'),
    register_ms: lib.func('void rdsparser_register_ms(void *rds, void *cb)'),
    register_ecc: lib.func('void rdsparser_register_ecc(void *rds, void *cb)'),
    register_country: lib.func('void rdsparser_register_country(void *rds, void *cb)'),
    register_af: lib.func('void rdsparser_register_af(void *rds, void *cb)'),
    register_ps: lib.func('void rdsparser_register_ps(void *rds, void *cb)'),
    register_rt: lib.func('void rdsparser_register_rt(void *rds, void *cb)'),
    register_ptyn: lib.func('void rdsparser_register_ptyn(void *rds, void *cb)'),
    register_ct: lib.func('void rdsparser_register_ct(void *rds, void *cb)'),
    string_get_content: lib.func(unicode_type + '* rdsparser_string_get_content(void *string)'),
    string_get_errors: lib.func('uint8_t* rdsparser_string_get_errors(void *string)'),
    string_get_length: lib.func('uint8_t rdsparser_string_get_length(void *string)'),
    ct_get_year: lib.func('uint16_t rdsparser_ct_get_year(void *ct)'),
    ct_get_month: lib.func('uint8_t rdsparser_ct_get_month(void *ct)'),
    ct_get_day: lib.func('uint8_t rdsparser_ct_get_day(void *ct)'),
    ct_get_hour: lib.func('uint8_t rdsparser_ct_get_hour(void *ct)'),
    ct_get_minute: lib.func('uint8_t rdsparser_ct_get_minute(void *ct)'),
    ct_get_offset: lib.func('int8_t rdsparser_ct_get_offset(void *ct)'),
    pty_lookup_short: lib.func('const char* rdsparser_pty_lookup_short(int8_t pty, bool rbds)'),
    pty_lookup_long: lib.func('const char* rdsparser_pty_lookup_long(int8_t pty, bool rbds)'),
    country_lookup_name: lib.func('const char* rdsparser_country_lookup_name(int country)'),
    country_lookup_iso: lib.func('const char* rdsparser_country_lookup_iso(int country)')
}

const decode_unicode = function(string) {
    let content = rdsparser.string_get_content(string);
    let length = rdsparser.string_get_length(string);
    if (length) {
        let array = koffi.decode(content, unicode_type + ' [' + length + ']');
        return String.fromCodePoint.apply(String, array);
    }
    return '';
};

const decode_errors = function(string) {
    let errors = rdsparser.string_get_errors(string);
    let length = rdsparser.string_get_length(string);
    if (length) {
        let array = koffi.decode(errors, 'uint8_t [' + length + ']');
        return Uint8Array.from(array).toString();
    }
    return '';
};

const callbacks = {
    pi: koffi.register(rds => (
        value = rdsparser.get_pi(rds),
        console.log('PI: ' + value.toString(16).toUpperCase())
    ), 'callback_pi*'),

    pty: koffi.register(rds => (
        value = rdsparser.get_pty(rds),
        display = rdsparser.pty_lookup_long(value, false),
        console.log('PTY: ' + display + ' (' + value + ')')
    ), 'callback_pty*'),

    tp: koffi.register(rds => (
        value = rdsparser.get_tp(rds),
        console.log('TP: ' + value)
    ), 'callback_tp*'),

    ta: koffi.register(rds => (
        value = rdsparser.get_ta(rds),
        console.log('TA: ' + value)
    ), 'callback_ta*'),

    ms: koffi.register(rds => (
        value = rdsparser.get_ms(rds),
        console.log('MS: ' + value)
    ), 'callback_ms*'),

    af: koffi.register((rds, value) => (
        console.log('AF: ' + value)
    ), 'callback_af*'),

    ecc: koffi.register(rds => (
        value = rdsparser.get_ecc(rds),
        console.log('ECC: ' + value.toString(16).toUpperCase())
    ), 'callback_ecc*'),

    country: koffi.register(rds => (
        value = rdsparser.get_country(rds),
        display = rdsparser.country_lookup_name(value),
        iso = rdsparser.country_lookup_iso(value),
        console.log('Country: ' + display + ' (' + iso + ')')
    ), 'callback_country*'),

    ps: koffi.register(rds => (
        ps = rdsparser.get_ps(rds),
        value = decode_unicode(ps),
        errors = decode_errors(ps),
        console.log('PS: ' + value +  '(' + errors + ')')
    ), 'callback_ps*'),

    rt: koffi.register((rds, flag) => (
        rt = rdsparser.get_rt(rds, flag),
        value = decode_unicode(rt),
        errors = decode_errors(rt),
        console.log('RT' + flag  + ': ' + value + ' (' + errors + ')')
    ), 'callback_rt*'),

    ptyn: koffi.register((rds, flag) => (
        ptyn = rdsparser.get_ptyn(rds),
        value = decode_unicode(ptyn),
        errors = decode_errors(ptyn),
        console.log('PTYN: ' + value + ' (' + errors + ')')
    ), 'callback_ptyn*'),

    ct: koffi.register((rds, ct) => (
        year = rdsparser.ct_get_year(ct),
        month = String(rdsparser.ct_get_month(ct)).padStart(2, '0'),
        day = String(rdsparser.ct_get_day(ct)).padStart(2, '0'),
        hour = String(rdsparser.ct_get_hour(ct)).padStart(2, '0'),
        minute = String(rdsparser.ct_get_minute(ct)).padStart(2, '0'),
        offset = rdsparser.ct_get_offset(ct),
        tz_sign = (offset >= 0 ? '+' : '-'),
        tz_hour = String(Math.abs(Math.floor(offset / 60))).padStart(2, '0'),
        tz_minute = String(Math.abs(offset % 60)).padStart(2, '0'),
        console.log('CT: ' + year + '-' + month + '-' + day + ' ' + hour + ':' + minute + ' (' + tz_sign + tz_hour + ':' + tz_minute + ')')
    ), 'callback_ct*')
}

let rds = rdsparser.new()
rdsparser.set_text_correction(rds, 0, 0, 2);
rdsparser.set_text_correction(rds, 0, 1, 2);
rdsparser.set_text_correction(rds, 1, 0, 2);
rdsparser.set_text_correction(rds, 1, 1, 2);
rdsparser.set_text_progressive(rds, 0, true);
rdsparser.set_text_progressive(rds, 1, true);
rdsparser.register_pi(rds, callbacks.pi);
rdsparser.register_pty(rds, callbacks.pty);
rdsparser.register_tp(rds, callbacks.tp);
rdsparser.register_ta(rds, callbacks.ta);
rdsparser.register_ms(rds, callbacks.ms);
rdsparser.register_ecc(rds, callbacks.ecc);
rdsparser.register_country(rds, callbacks.country);
rdsparser.register_af(rds, callbacks.af);
rdsparser.register_ps(rds, callbacks.ps);
rdsparser.register_rt(rds, callbacks.rt);
rdsparser.register_ptyn(rds, callbacks.ptyn);
rdsparser.register_ct(rds, callbacks.ct);

let data = [
    "A20120017420696E02",
    "A201E0153475A20313",
    "A20130100064CD4615",
    "A2018001D9F579E800",
    "A2010019032B4F4500",
    "A201200220D7313A00",
    "A201E015067BA20318",
    "A201001A2B5F203114",
    "A2018001D9F579E801",
    "A201001F1F2B202001",
    "A201E0157907A20300",
    "A2010018E703202000",
    "A20120032044696500",
    "A2018001D9F579E834",
    "A2010019032B4F4500",
    "A201E015640DA20300",
    "A201001A0320203100",
    "A20180014E91920000",
    "A201001F035F202000",
    "A201E015026AA20310",
    "A2010018833120200C",
    "A20120042066656D04",
    "A2014001D758F84233",
    "A20180014E91920010",
    "A2010019015F4F4511",
    "A2012005696E697321",
    "A201E0153079A20314",
    "A2013010580CCD463D",
    "A20180014E91920004",
    "A201001AE51E203100",
    "A20120067469736300",
    "A201E0152078A20300",
    "A201001F1E5F202024",
    "A2018002DE418CBE01",
    "A2010018021E202000",
    "A201E0151F8CA2031C",
    "A2010019E5024F4502",
    "A20120076865204200",
    "A2018002DE418CBE00",
    "A201001A025F203102",
    "A201E0150F6AA20308",
    "A201001F021E202002",
    "A2018002DE418CBE14",
    "A2010018E32220200C",
    "A201E0155F13A20329",
    "A2010019225F4F4500",
    "A201200861726F6300",
    "A20180025418EE9000",
    "A201001AE921203100",
    "A20120096B64696310",
    "A201E0153A05A20300",
    "A20130100064CD4600",
    "A20180025418EE9000",
    "A201001F215F202000",
    "A201200A6874657200",
    "A201E01D0000A20301",
    "A20100181C21202000",
    "A20180025418EE9000",
    "A201001921314F4500",
    "A201E0002020A21300",
    "A201001A2123203102",
    "A201200B696E205300",
    "A20180020CA0000002",
    "A201001FE51C202005",
    "A201E001464DA21300",
    "A20100181C21202002",
    "A20180020CA0000001",
    "A20100191C5F4F4500",
    "A201E0023420A21304",
    "A201001AE931203130",
    "A201200C6962796C00",
    "A20180020CA0000008",
    "A201001F2131202000",
    "A201200D6C61205310",
    "A201E0032020A21310",
    "A20130104000CD4612",
    "A201800843F4A68735",
    "A2010018315F202000",
    "A201200E6368776100",
    "A201E0055F71A21316",
    "A201001925314F4524",
    "A201800843F4A68704",
    "A201001A3180203101",
    "A201E0050192A21300",
    "A201001FE90F202004",
    "A201200F727A202000",
    "A201800843F4A68708",
    "A20100180F5F202011",
    "A201E005348BA21300",
    "A20100190F1E4F451A",
    "A2018003C191BF2C20",
    "A201001A0F23203127",
    "A201E0052DA3A21304",
    "A201001F0F32202001",
    "A20120005269747A08",
    "A2018003C191BF2C06",
    "A2010018E51E202000",
    "A20120017420696E00",
    "A2013834F68DA2133C",
    "A2018003C191BF2C09",
    "A20100191E5F4F4514",
    "A201200220D7313A00",
    "A201E00564A5A21320",
    "A201001A0F1E203100",
    "A2018003415D2C8C04",
    "A201001FF95F202001",
    "A201E00D0001A21300",
    "A20100182D5F202010",
    "A2012003204469650C",
    "A2018003415D2C8C3C",
    "A20100195F644F4500",
    "A20130100064CD4606",
    "A201E0005241A60210",
    "A201064A345F203122",
    "A2018003415D2C8C0C",
    "A201001F1E5F202000",
    "A201E0014449A60214",
    "A2010018225F202000",
    "A20120042066656D00",
    "A20180085065A0FC30",
    "A2010019375F4F4504",
    "A2012005696E697330",
    "A201E62A492EA6022C",
    "A20180085065A0FC00",
    "A201001A305F203110",
    "A2012006176F73630C",
    "A201E0034E20A60201",
    "A201001F3A5F20200C",
    "A20180085065A14F3B",
    "A20100180F5F202020",
    "A201F9B5C754A60235",
    "A2010019025F4F4501",
    "A201200768C320422C",
    "A2018004B87D8DB010",
    "A201001A015F203101",
    "A20130104000CD4603",
    "A201E0051E39A602C0",
    "A201001F215F202000",
    "A201800488658DB000",
    "A2010018F15F202020",
    "A201E0051E3EA60204",
    "A20100192B5F40872F",
    "A201200861726F6300",
    "A201800488658DB0D4",
    "A201001A5F792031C0",
    "A20120096B64696306",
    "A201E0053452A60208",
    "A20180045BA000002F",
    "A201001F065F202000",
    "A201200A6874657237",
    "A201E005A47AA60204",
    "A2010018265F202000",
    "A2018004421000000D",
    "A2010019395F4F4510",
    "A201E0055F28A60200",
    "A201001A035F203130",
    "A201200B696E205304",
    "A2018004A20800001C",
    "A201001F1C5F202010",
    "A201051B0064CD4634",
    "A201E6062D68A6023E",
    "A2010018315F202002",
    "A20180058991C1E110",
    "A2010019E5374F4543",
    "A201E0053C4EA60204",
    "A201001A2D37203100",
    "A201200C6962796C00",
    "A20180058991C1E100",
    "A201001F375F202000",
    "A201200D6C61205300",
    "A201E005223FA60200",
    "A20130104000CD4634",
    "A20180058991C1E100",
    "A2010018E330202030",
    "A201200E6F6E440107",
    "A201E0052661A6022C",
    "A2010019305F4F451B",
    "A20180054E96460000",
    "A201001AE534203100",
    "A201A0005445535400", // PTYN
    "A201A0015445535400", // PTYN
    "A2014001D750018200", // CT
    "A201100000E0000000", // ECC
]

for (let group of data) {
    rdsparser.parse_string(rds, group);
}

for (let cb in callbacks) {
    koffi.unregister(callbacks[cb]);
}

rdsparser.free(rds);
