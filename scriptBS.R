bs = read.table('bsData.csv')
lg <- function(x)c(NA, x[1:(length(x)-1)])
bs$R = (log(bs$V1) - log(lg(bs$V1)))
plot(density(bs$R,na.rm = TRUE),
     main = "Densité des log-rentabilités générées")

bs2 = read.table('bsData2.csv')
bs2$R1 = (log(bs2$V1) - log(lg(bs2$V1)))
bs2$R2 = (log(bs2$V2) - log(lg(bs2$V2)))
cor(na.omit(bs2$R1),na.omit(bs2$R2))

bs3 = read.table('callPriceData.txt')
plot(density(bs3$V1),
     main = "Densité de l'écart entre le prix estimé et le prix B&S d'un Call\n(S=10, K=14, r=0.05, sigma=0.3)")

bs4 = read.table('callStrikeData.txt')
plot(bs4$V1,bs4$V2,
     type = 'o',
     col = "blue",
     xlab = "Strike",
     ylab = "Price",
     main="Prix d'un Call en fonction du strike (S=100,T=1,r=0.05,sigma=0.03)")
lines(bs4$V1,bs4$V3,
      col = "red")
legend(150,80,c("Prix Monte-Carlo","Prix Black-Scholes"),cex=1,col=c("blue","red"), pch=c(21,-1), lty=1:1)

