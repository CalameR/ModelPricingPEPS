//
// Created by calamer on 1/19/17.
//

#ifndef MC_PRICER_POOL_PATTERN_H
#define MC_PRICER_POOL_PATTERN_H

#ifdef _OPENMP
#include <omp.h>
#endif
#include <ctime>
#include <algorithm>

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

/**
 * A pool of objects to be used in thread-safe programs
 *
 * Be sure to call the ::init() method after creating a pool of objects.
 *
 * @tparam _T the type of abjects
 * @tparam _Alloc the Allocator to create the objects
 *
 */
template <typename _T, typename _Alloc> class Pool
{
private:
    _T *data; /// The array of data
    int nThreads; /// Number of threads, ie. number of elements in data
    _Alloc allocator; /// An instance of the allocator class

public:
    Pool()
    {
        data = new _T[0];
        nThreads = 0;
    }

    ~Pool()
    {
        for (int i = 0 ; i < nThreads ; i++)
        {
            allocator.destroy(&(data[i]));
        }
        delete [] data;
    }

    /**
     * With OpenMP on, return the instance indexed by the thread number.
     * With OpenMP off, returns the single instance.
     *
     * @return the current workspace
     */
    _T operator()()
    {
#ifdef _OPENMP
        return operator()(omp_get_thread_num());
#else
        return operator()(0);
#endif
    }

    /**
     * @param i an integer between 0 and nThreads
     *
     * @return data[i]
     */
    _T operator()(int i)
    {
#ifndef PNL_RANGE_CHECK_OFF
        if (i > nThreads)
        {
            printf("Accessing too many workspaces\n");
            abort();
        }
#endif
        return data[i];
    }

    /**
     * With OpenMP on, set nThreads to the maximum number of threads OMP can create
     * With OpenMP off, set nThreads to 1
     */
    void init(int size1, int size2)
    {
#ifdef _OPENMP
        set_max_threads(size1,size2,omp_get_max_threads());
#else
        set_max_threads(1);
#endif
    }

    /**
     * Set the maximul number of threads
     *
     * @param n an integer. When the number of threads changes, the old elements
     * are kept. New elements are created by calling allocator.create()
     */
    void set_max_threads(int size1,int size2,int n)
    {
        _T *new_data = new _T[n];
        // Copy as much as possible
        for (int i = 0; i < std::min(nThreads, n); i++)
        {
            new_data[i] = data[i];
        }
        // Create new elements if any.
        for (int i = nThreads; i < n; i++)
        {
            allocator.nThread = i;
            new_data[i] = allocator.create(size1,size2);
        }
        delete [] data;
        nThreads = n;
        data = new_data;
    }
};

/**
 * Allocator structure for PnlVect*
 */
struct AllocPnlVect
{
    int nThread;
    PnlVect *create(int size1, int size2)
    {
        return pnl_vect_create(size1);
    }
    void destroy(PnlVect **v)
    {
        pnl_vect_free(v);
    }
};

/**
 * Allocator structure for PnlMat*
 */
struct AllocPnlMat
{
    int nThread;
    PnlMat *create(int size1, int size2)
    {
        return pnl_mat_create(size1,size2);
    }
    void destroy(PnlMat **M)
    {
        pnl_mat_free(M);
    }
};

/**
 * Allocator structure for PnlRng*. The created random number generators are
 * independent and suitable for usage with OpenMP.
 */
struct AllocPnlRng
{
    int nThread;
    PnlRng *create(int size1, int size2)
    {
        PnlRng *rng = pnl_rng_dcmt_create_id(nThread, 3128);
        pnl_rng_sseed(rng, time(NULL));
        return rng;
    }
    void destroy(PnlRng **rng)
    {
        pnl_rng_free(rng);
    }
};

typedef Pool<PnlVect *, AllocPnlVect> PnlVect_Pool;
typedef Pool<PnlMat *, AllocPnlMat> PnlMat_Pool;
typedef Pool<PnlRng *, AllocPnlRng> PnlRng_Pool;
#endif //MC_PRICER_POOL_PATTERN_H
