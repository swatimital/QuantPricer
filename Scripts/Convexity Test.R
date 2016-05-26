library(plot3D)
#library(rgl)
BSPrice <- function(St, K, r, t, T, vol, call)
{
  d1 <- (log(St/K) + (r + vol*vol*0.5)*(T-t))/(vol*sqrt(T-t))
  d2 <- (log(St/K) + (r - vol*vol*0.5)*(T-t))/(vol*sqrt(T-t))
  
  if (call == TRUE)
  {
    return (St*pnorm(d1) - K*exp(-r*(T-t))*pnorm(d2))
  }
  else
  {
    return(K*exp(-r*(T-t))*pnorm(-d2) - St*pnorm(-d1))
  }
}

Gamma <- function(St, K, r, t, T, vol)
{
  d1 <- (log(St/K) + (r + vol*vol*0.5)*(T-t))/(vol*sqrt(T-t))
  return (dnorm(d1)/(St*vol*sqrt(T-t)))
}

BSPrice(100.0, 100.0, 0.05, 0, 1.0, 0.0, TRUE)
Gamma(100.0, 100.0, 0.05, 0, 1.0, 0.1)

max.price <- 500
maturity <- 30
K <- 350
rf <- 0.05
vol <- 0.1

stock.prices <- seq(5, max.price, by=15)
time <- seq(0, maturity-1, by=5)

list.prices <- c()
for (price in stock.prices)
{
  for (t in time)
  {
    list.prices <- c(list.prices, BSPrice(price, K, rf, t, maturity, vol, TRUE))
  }
}
prices <- matrix(list.prices, ncol=length(stock.prices), nrow=length(time))

persp3D(x = time, y = stock.prices, z = prices, theta=90)
plot(stock.prices, prices[5,], type='l')