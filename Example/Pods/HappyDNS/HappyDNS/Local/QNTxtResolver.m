//
//  QNTxtResolver.m
//  HappyDNS
//
//  Created by bailong on 16/1/5.
//  Copyright © 2016年 Qiniu Cloud Storage. All rights reserved.
//

#import "QNTxtResolver.h"

#include <arpa/inet.h>
#include <resolv.h>
#include <string.h>

#import "QNDomain.h"
#import "QNRecord.h"
#import "QNResolver.h"

@interface QNTxtResolver ()
@property (nonatomic) NSString *address;
@end

static NSArray *query_ip(res_state res, const char *host) {
    u_char answer[1500];
    int len = res_nquery(res, host, ns_c_in, ns_t_txt, answer, sizeof(answer));

    ns_msg handle;
    ns_initparse(answer, len, &handle);

    int count = ns_msg_count(handle, ns_s_an);
    if (count != 1) {
        res_ndestroy(res);
        return nil;
    }

    char txtbuf[256];
    memset(txtbuf, 0, sizeof(txtbuf));

    ns_rr rr;
    if (ns_parserr(&handle, ns_s_an, 0, &rr) != 0) {
        res_ndestroy(res);
        return nil;
    }
    int t = ns_rr_type(rr);
    int ttl = ns_rr_ttl(rr);
    int rdlen = ns_rr_rdlen(rr);
    if (rdlen <= 1 + 7) {
        res_ndestroy(res);
        return nil;
    }
    NSString *val;
    if (t == ns_t_txt) {
        memcpy(txtbuf, ns_rr_rdata(rr) + 1, rdlen - 1);
        val = [NSString stringWithUTF8String:txtbuf];
    } else {
        res_ndestroy(res);
        return nil;
    }
    NSArray *ipArray = [val componentsSeparatedByString:@","];
    NSMutableArray *ret = [[NSMutableArray alloc] initWithCapacity:ipArray.count];
    for (int i = 0; i < ipArray.count; i++) {
        QNRecord *record = [[QNRecord alloc] init:[ipArray objectAtIndex:i] ttl:ttl type:kQNTypeA];
        [ret addObject:record];
    }

    res_ndestroy(res);
    return ret;
}

static int setup_dns_server_v4(res_state res, const char *dns_server) {
    struct in_addr addr;
    int r = inet_pton(AF_INET, dns_server, &addr);
    if (r == 0) {
        return -1;
    }

    res->nsaddr_list[0].sin_addr = addr;
    res->nsaddr_list[0].sin_family = AF_INET;
    res->nsaddr_list[0].sin_port = htons(NS_DEFAULTPORT);
    res->nscount = 1;
    return 0;
}

// does not support ipv6 nameserver now
static int setup_dns_server_v6(res_state res, const char *dns_server) {
    return -1;
}

static int setup_dns_server(res_state res, const char *dns_server) {
    int r = res_ninit(res);
    if (r != 0) {
        return r;
    }
    if (dns_server == NULL) {
        return 0;
    }

    if (strchr(dns_server, ':') == NULL) {
        return setup_dns_server_v4(res, dns_server);
    }

    return setup_dns_server_v6(res, dns_server);
}

@implementation QNTxtResolver
- (instancetype)initWithAddres:(NSString *)address {
    if (self = [super init]) {
        _address = address;
    }
    return self;
}

- (NSArray *)query:(QNDomain *)domain networkInfo:(QNNetworkInfo *)netInfo error:(NSError *__autoreleasing *)error {
    struct __res_state res;

    int r;
    if (_address == nil) {
        r = setup_dns_server(&res, NULL);
    } else {
        r = setup_dns_server(&res, [_address cStringUsingEncoding:NSASCIIStringEncoding]);
    }
    if (r != 0) {
        return nil;
    }

    return query_ip(&res, [domain.domain cStringUsingEncoding:NSUTF8StringEncoding]);
}

@end
