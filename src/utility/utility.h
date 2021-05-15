#pragma once

// 解决Qt常亮中文乱码, 已经不需要, 新的解决办法是
// 安装 Format on Save 扩展, 让VS的文件默认是UTF8编码格式
// 就可以解决乱码问题
#define CC(ctx) QStringLiteral(ctx).toUtf8().constData()
#define CV(ctx) QString::fromLocal8Bit(ctx).toUtf8().data()

#define QUOTE(TXT) QString("'%1'").arg(TXT)
#define QUOTE_S(TXT) QString("(%1)").arg(TXT)
#define QUOTE_M(TXT) QString("[%1]").arg(TXT)
#define QUOTE_L(TXT) QString("{%1}").arg(TXT)
