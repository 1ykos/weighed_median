#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

using std::abs;
using std::array;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::get;
using std::getline;
using std::isfinite;
using std::isinf;
using std::isnan;
using std::make_tuple;
using std::round;
using std::sort;
using std::stod;
using std::stoull;
using std::streamsize;
using std::string;
using std::stringstream;
using std::swap;
using std::to_string;
using std::transform;
using std::tuple;
using std::vector;

const double gauss_cdf(
    const double x,
    const double m,
    const double v
    )
{
  return 0.5*(1+erf((x-m)/sqrt(2*v)));
}

const double gauss_integral(
    const double m,
    const double v,
    const double a,
    const double b
    )
{
//return ((a<b)-(b<a))*abs(gauss_cdf(b,m,v)-gauss_cdf(a,m,v));
  return gauss_cdf(b,m,v)-gauss_cdf(a,m,v);
}

int main(int argc,char** argv) {
  vector<tuple<double,double>> data;
  double sumw = 0;
  double sumw2= 0;
  for (string line;getline(cin,line);) {
    double v,w;
    stringstream ss(line);
    ss >> v >> w;
    if (!ss) break;
    data.emplace_back(v,w);
    sumw += w;
    sumw2+= w*w;
  }
  if (sumw==0) return 1;
  sort(data.begin(),data.end());
  double lower_median = 0;
  double upper_median = 0;
  double t = 0;
  for (auto it=data.begin();it!=data.end();++it) {
    //cerr << get<0>(*it) << " " << get<1>(*it) << " " << t << endl;
    if (2*(t+get<1>(*it))==sumw) {
      lower_median = get<0>(*it);
    }
    if (2*(t+get<1>(*it))>sumw) {
      if (2*t<sumw) lower_median = get<0>(*it);
      upper_median = get<0>(*it);
      break;
    }
    t+=get<1>(*it);
  }
  // lower_median and upper_median are almost always equal. In case they are
  // not, choosing any one of the two, if not randomly, will bias the overall
  // weigted sample median. And so, for real numbers, it is sensible in most
  // cases to take the average between the two. Certain applications however
  // might require another way to resolve this ambiguity.
  // cerr << lower_median << " " << upper_median << endl;
  const double median = 0.5*(upper_median+lower_median);
  double variance = 0;
  double normalization = 0;
  t = 0;
  for (auto it=data.begin();it!=data.end();++it) {
    const double a = t;
    const double b = (t+=get<1>(*it));
    const double w = gauss_integral(0.5*sumw,0.25*sumw2,a,b);
    variance += w*(get<0>(*it)-median)*(get<0>(*it)-median);
    normalization += w;
  }
  cout << median << " " << variance << endl;
  // normalization does not make a difference
  // cout << median << " " << variance/normalization << endl;
}
