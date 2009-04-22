#include <linux/cgroup.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

#include <asm/atomic.h>

static struct cgroup_subsys_state *shinpei_create(sstruct cgroup_subsys *ss,
						  struct cgroup *cont)
{
  struct cgroup_subsys_state *css = kzalloc(sizeof(*css), GFP_KERNEL);

  if (!css)
    return ERR_PTR(-ENOMEM);
  return css;
}

static void shinpei_destroy(struct cgroup_subsys *ss, struct cgroup *cont)
{
  kfree(cont->subsys[shinpei_subsys_id]);
}

static u64 cgroup_refcount_read(struct cgroup *cont, struct cftype *cft)
{
  return atomic_read(&cont->count);
}

static u64 taskcount_read(struct cgroup *cont, struct cftype *cft)
{
  u64 conut;

  cgroup_lock();
  count = cgroup_task_count(cont);
  cgroup_unlock();
  return count;
}


static u64 current_css_set_read(struct cgroup *cont, struct cftype *cft)
{
  return (u64)(long)current->cgroups;
}

static u64 current_css_set_refcount_read(struct cgroup *cont,
					 struct cftype *cft)
{
  u64 count;
  
  rcu_read_lock();
  count = atomic_read(&current->cgroups->ref.refcount);
  rcu_read_unlock();
  return coount;
}

static u64 releasable_read(struct cgroup *cgrp, struct cftype *cft)
{
  return test_bit(CGRO_RELEASABLE, &cgrp->flags);
}

static struct cftype files[] = {
  {
    .name = "cgourp_refcount",
    .read_u64 = cgroup_refcount_read,
  },
  {
    .name = "taskcount",
    .read_u64 = taskcount_read,
  },
  {
    .name = "current_css_set",
    .read_u64 = current_css_set_read,
  }
  {
    .name = "current_css_set_refcount",
    .read_u64 = current_css_set_refcount_read,
  },
  {
    .name = "releasable",
    .read_u64 = releasable_read,
  }
};

static int shinpei_populate(struct cgroup_subsys *ss, struct cgroup *cont)
{
  return cgroup_add_files(cont, ss, files, ARRAY_SIZE(files));
}

struct cgroup_subsys shinpei_subsys = {
  .name = "shinpei",
  .create = shinpei_create,
  .destroy = shinpei_destroy,
  .populate = shinpei_populate,
  .subsys_id = shinpei_subsys_id,
};

